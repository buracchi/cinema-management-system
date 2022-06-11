#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>

#include <buracchi/common/utilities/utilities.h>
#include <buracchi/common/utilities/try.h>
#include <fort.h>
#include <cms/booking.h>

#include "utilities/io.h"

#define INT32DSTR_LEN 17

static const char* title = "*********************************\n\
						  \r*       SCREENINGS BOOKER       *\n\
						  \r*********************************\n";

struct booking_data {
	uint32_t cinema_id;
	char cinema_address[128];
	uint8_t hall;
	char date[DATE_LEN];
	char time[TIME_LEN];
	char film_name[45];
	char price[18];
	char seat_row;
	uint8_t seat_number;
	uint32_t booking_code;
};

static int choose_cinema(cms_t cms, struct booking_data* booking_data);
static int choose_screening(cms_t cms, struct booking_data* booking_data);
static int choose_seat(cms_t cms, struct booking_data* booking_data);
static int make_payment(cms_t cms, struct booking_data* booking_data);

static int print_cinema_number_header(int cols);
static int print_screen(int cols);

extern int make_booking(cms_t cms) {
	struct booking_data booking_data = { cms };
	try(choose_cinema(cms, &booking_data), 1, fail);
	try(choose_screening(cms, &booking_data), 1, fail);
	try(choose_seat(cms , &booking_data), 1, fail);
	try(make_payment(cms , &booking_data), 1, fail);
	printf("\nYou booking code is: %d\n", booking_data.booking_code);
	printf("The reservation code will be required to access the cinema and take advantage of the reservation or to request a refund.\n");
	printf("Save it in a safe place and be careful not to lose it.\n");
	press_anykey();
	return 0;
fail:
	return 1;
}

static int choose_cinema(cms_t cms, struct booking_data* booking_data) {
	struct cms_get_all_cinema_response* response;
	ft_table_t* table;
	if (cms_get_all_cinema(cms, &response)) {
		printf("%s", response->error_message);
	}
	io_clear_screen();
	puts(title);
	table = ft_create_table();
	ft_set_cell_prop(table, 0, FT_ANY_COLUMN, FT_CPROP_ROW_TYPE, FT_ROW_HEADER);
	ft_write_ln(table, "CINEMA", "ADDRESS", "OPENING TIME", "CLOSING TIME");
	for (uint64_t i = 0; i < response->num_elements; i++) {
		char id[INT32DSTR_LEN];
		ft_write_ln(table,
			itoa(response->result[i].id, &id, 10),
			response->result[i].address,
			response->result[i].opening_time,
			response->result[i].closing_time);
	}
	printf("%s\n", ft_to_string(table));
	ft_destroy_table(table);
	char selected_cinema[INT32DSTR_LEN];
	get_input("Select a cinema: ", selected_cinema, false);
	booking_data->cinema_id = atoi(selected_cinema);
	strcpy(booking_data->cinema_address, response->result[booking_data->cinema_id - 1].address);
	cms_destroy_response((struct cms_response*)response);
	return 0;
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
	for (int i = 0; i < response->num_elements; i++) {
		char screening_id[INT32DSTR_LEN];
		char hall_id[INT32DSTR_LEN];
		ft_write_ln(table,
			itoa(i + 1, &screening_id, 10),
			response->result[i].date,
			response->result[i].time,
			itoa((int)response->result[i].hall_id, &hall_id, 10),
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
	strcpy(request.date, booking_data->date);
	strcpy(request.start_time, booking_data->time);
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
		for (int i = 0; i < num_rows; i++) {
			printf(" %c| ", 'A' + i);
			for (int j = 0; j < num_cols; j++) {
				bool available = false;
				for (int k = 0; k < response->num_elements; k++) {
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
		for (int i = 0; i < response->num_elements; i++) {
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
	strcpy(request.date, booking_data->date);
	strcpy(request.start_time, booking_data->time);
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
	strcpy(request.name_on_card, name_on_card);
	strcpy(request.card_number, card_number);
	strcpy(request.expiry_date, expiry_date);
	strcpy(request.security_code, security_code);
	if (cms_book_seat(cms, request, &response)) {
		printf("%s", response->error_message);
	}
	booking_data->booking_code = response->result[0].booking_code;
	cms_destroy_response((struct cms_response*)response);
	return 0;
}

static int print_cinema_number_header(int cols) {
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

static int print_screen(int cols) {
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
