#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>

#include <buracchi/common/utilities/utilities.h>
#include <buracchi/common/utilities/try.h>
#include <fort.h>
#include <cms/booking.h>

#include "utilities/io.h"
#include "utilities/strto.h"
#include "resources.h"

#define INT32DSTR_LEN 12
#define UINT64DSTR_LEN 22

struct booking_data {
	int32_t cinema_id;
	char cinema_address[128];
	uint8_t hall;
	char date[DATE_LEN];
	char time[TIME_LEN];
	char film_name[45];
	char price[18];
	char seat_row;
	uint8_t seat_number;
	int32_t booking_code;
};

static int choose_cinema(cms_t cms, struct booking_data* booking_data);
static int choose_screening(cms_t cms, struct booking_data* booking_data);
static int choose_seat(cms_t cms, struct booking_data* booking_data);
static int make_payment(cms_t cms, struct booking_data* booking_data);

static inline int print_cinema_number_header(int cols);
static inline int print_screen(int cols);
static const char* get_cinema_table(struct cms_get_all_cinema_response* response);

extern int make_booking(cms_t cms) {
	struct booking_data booking_data = { 0 };
	try(choose_cinema(cms, &booking_data), 1, fail);
	try(choose_screening(cms, &booking_data), 1, fail);
	try(choose_seat(cms , &booking_data), 1, fail);
	try(make_payment(cms , &booking_data), 1, fail);
	printf("\nIl codice di prenotazione è: %d\n", booking_data.booking_code);
	puts("Il codice di prenotazione è necessario per accedere al cinema ed usufruire della prenotazione o per effettuarne il rimborso.");
	puts("Salvarlo in un luogo sicuro ed assicurarsi di non perderlo.");
	press_anykey();
	return 0;
fail:
	return 1;
}

static int choose_cinema(cms_t cms, struct booking_data* booking_data) {
	struct cms_get_all_cinema_response* response;
	const char* str_table;
	char selected_cinema[INT32DSTR_LEN];
	try(cms_get_all_cinema(cms, &response), 1, fail);
	if (response->error_message) {
		printf("%s", response->error_message);
		goto fail2;
	}
	try(str_table = get_cinema_table(response), NULL, fail2);
	io_clear_screen();
	puts(title);
	puts(str_table);
	get_input("Selezionare un cinema: ", selected_cinema, false);
	try(strtoint32(&(booking_data->cinema_id), selected_cinema, 10) == STRTO_SUCCESS, false, fail2);
	strcpy(booking_data->cinema_address, response->result[booking_data->cinema_id - 1].address);
	cms_destroy_response((struct cms_response*)response);
	return 0;
fail2:
	cms_destroy_response((struct cms_response*)response);
fail:
	return 1;
}

static int choose_screening(cms_t cms, struct booking_data* booking_data) {
	struct cms_get_cinema_screenings_request request = { .cinema_id = booking_data->cinema_id };
	struct cms_get_cinema_screenings_response* response;
	ft_table_t* table;
	if (cms_get_cinema_screenings(cms, request, &response)) {
		fprintf(stderr, "%s", response->error_message);
		exit(1);
	}
	io_clear_screen();
	puts(title);
	printf("Selected cinema: %s\n\n", booking_data->cinema_address);
	table = ft_create_table();
	ft_set_cell_prop(table, 0, FT_ANY_COLUMN, FT_CPROP_ROW_TYPE, FT_ROW_HEADER);
	ft_write_ln(table, "SCREENING", "DATE", "TIME", "HALL", "FILM", "RUNNING TIME", "PRICE");
	for (uint64_t i = 0; i < response->num_elements; i++) {
		char screening_id[UINT64DSTR_LEN];
		char hall_id[INT32DSTR_LEN];
		snprintf(screening_id, UINT64DSTR_LEN, "%lu", i + 1);
		snprintf(hall_id, INT32DSTR_LEN, "%d", response->result[i].hall_id);
		ft_write_ln(table,
			screening_id,
			response->result[i].date,
			response->result[i].time,
			hall_id,
			response->result[i].film_name,
			response->result[i].running_time,
			response->result[i].price);
	}

	printf("%s\n", ft_to_string(table));
	ft_destroy_table(table);
	char selected_screening[INT32DSTR_LEN];
	get_input("Select a screening: ", selected_screening, false);
	int i = atoi(selected_screening) - 1;
	booking_data->hall = response->result[i].hall_id;
	strcpy(booking_data->date, response->result[i].date);
	strcpy(booking_data->time, response->result[i].time);
	strcpy(booking_data->film_name, response->result[i].film_name);
	strcpy(booking_data->price, response->result[i].price);
	cms_destroy_response((struct cms_response*)response);
	return 0;
}

static int choose_seat(cms_t cms, struct booking_data* booking_data) {
	struct cms_get_cinema_halls_request halls_request = {
		.cinema_id = booking_data->cinema_id
	};
	struct cms_get_cinema_halls_response* halls_response;
	struct cms_get_available_seats_request request = {
		.cinema_id = booking_data->cinema_id,
		.hall_id = booking_data->hall,
	};
	strcpy((char*) request.date, booking_data->date);
	strcpy((char*) request.start_time, booking_data->time);
	struct cms_get_available_seats_response* response;
	char selected_row[2];
	char selected_number[INT32DSTR_LEN];
	bool selected_seat_available = false;
	do {
		if (!cms_get_cinema_halls(cms, halls_request, &halls_response)) {
			printf("%s", halls_response->error_message);
		}
		if (!cms_get_available_seats(cms, request, &response)) {
			printf("%s", response->error_message);
		}
		uint64_t num_rows = halls_response->result[booking_data->hall].num_rows;
		uint64_t num_cols = halls_response->result[booking_data->hall].num_cols;
		io_clear_screen();
		puts(title);
		printf("Selected date: %s\n", booking_data->date);
		printf("Selected time: %s\n", booking_data->time);
		printf("Selected cinema: %s\n", booking_data->cinema_address);
		printf("Selected hall: %hhu\n", booking_data->hall);
		printf("Selected film: %s\n", booking_data->film_name);
		printf("Price: %s\n", booking_data->price);
		printf("\n");
		printf("\nLegend:\na - available seat\nr - reserved seat\n\n");
		print_cinema_number_header(num_cols);
		for (uint64_t i = 0; i < num_rows; i++) {
			printf(" %c| ", (int) ('A' + i));
			for (uint64_t j = 0; j < num_cols; j++) {
				bool available = false;
				for (uint64_t k = 0; k < response->num_elements; k++) {
					if (response->result[k].row == ('A' + i) && response->result[k].number == (j + 1)) {
						available = true;
						break;
					}
				}
				printf(" %s ", available ? "a" : "r");
			}
			printf("|\n");
		}
		print_screen(num_cols);
		get_input("Select a row [A,...]: ", selected_row, false);
		get_input("Select a number: ", selected_number, false);
		int snumber = atoi(selected_number);
		for (uint64_t i = 0; i < response->num_elements; i++) {
			if (response->result[i].row == selected_row[0] && response->result[i].number == snumber) {
				selected_seat_available = true;
				break;
			}
		}
	} while (!selected_seat_available);
	booking_data->seat_row = selected_row[0];
	booking_data->seat_number = atoi(selected_number);
	cms_destroy_response((struct cms_response*)response);
	cms_destroy_response((struct cms_response*)halls_response);
	return 0;
}

static int make_payment(cms_t cms, struct booking_data* booking_data) {
	char name_on_card[128];
	char card_number[17];
	char expiry_date[DATE_LEN];
	char security_code[4];
	struct cms_book_seat_response* response;
	struct cms_book_seat_request request = {
		.cinema_id = booking_data->cinema_id,
		.hall_id = booking_data->hall,
		.seat_row = booking_data->seat_row,
		.seat_number = booking_data->seat_number
	};
	strcpy((char*) request.date, booking_data->date);
	strcpy((char*) request.start_time, booking_data->time);
	io_clear_screen();
	puts(title);
	printf("Selected date: %s\n", booking_data->date);
	printf("Selected time: %s\n", booking_data->time);
	printf("Selected cinema: %s\n", booking_data->cinema_address);
	printf("Selected hall: %hhu\n", booking_data->hall);
	printf("Selected film: %s\n", booking_data->film_name);
	printf("Selected seat: %c%hhu\n", booking_data->seat_row, booking_data->seat_number);
	printf("Price: %s\n", booking_data->price);
	printf("\n");
	get_input("Enter name on card: ", name_on_card, false);
	get_input("Enter card number: ", card_number, false);
	get_input("Enter expiry date [YYYY-MM]: ", expiry_date, false);
	get_input("Enter secuity code (CVV): ", security_code, true);
	strcpy((char*) request.name_on_card, name_on_card);
	strcpy((char*) request.card_number, card_number);
	strcpy((char*) request.expiry_date, expiry_date);
	strcpy((char*) request.security_code, security_code);
	if (cms_book_seat(cms, request, &response)) {
		printf("%s", response->error_message);
	}
	booking_data->booking_code = response->result[0].booking_code;
	cms_destroy_response((struct cms_response*)response);
	return 0;
}

static inline int print_cinema_number_header(int cols) {
	printf("  | ");
	for (int i = 0; i < cols; i++) {
		printf("%s%d ", i < 9 ? " " : "", i + 1);
	}
	printf("|\n");
	printf("--+-");
	for (int i = 0; i < cols; i++) {
		printf("---");
	}
	printf("+\n");
	return 0;
}

static inline int print_screen(int cols) {
	printf("--+###");
	for (int i = 0; i < cols - 1; i++) {
		printf("###");
	}
	printf("#+\n  |#");
	printf("%*sSCREEN%*s",
		(cols * 3 - 8) / 2 + ((cols % 2) ? 2 : 1), "",
		(cols * 3 - 8) / 2, "");
	printf("#|\n");
	printf("  |###");
	for (int i = 0; i < cols - 1; i++) {
		printf("###");
	}
	printf("#|\n");
	return 0;
}

static const char* get_cinema_table(struct cms_get_all_cinema_response* response) {
	const char* result;
	ft_table_t* table;
	try(table = ft_create_table(), NULL, fail);
	try(ft_set_cell_prop(table, 0, FT_ANY_COLUMN, FT_CPROP_ROW_TYPE, FT_ROW_HEADER) < 0, true, fail2);
	try(ft_write_ln(table, "CINEMA", "INDIRIZZO", "APERTURA", "CHIUSURA") < 0, true, fail2);
	for (uint64_t i = 0; i < response->num_elements; i++) {
		char id[INT32DSTR_LEN] = { 0 };
		char* address = response->result[i].address;
		char* opening = response->result[i].opening_time;
		char* closing = response->result[i].closing_time;
		try(snprintf(id, INT32DSTR_LEN, "%d", response->result[i].id) < 0, true, fail2);
		try(ft_write_ln(table, id, address, opening, closing) < 0, true, fail2);
	}
	try(result = ft_to_string((const ft_table_t*)table), NULL, fail2);
	ft_destroy_table(table);
	return result;
fail2:
	ft_destroy_table(table);
fail:
	return NULL;
}
