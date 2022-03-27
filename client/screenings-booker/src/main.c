#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>

#include <buracchi/common/utilities/utilities.h>
#include <buracchi/common/utilities/try.h>
#include <fort.h>

#include "cinema-management-service.h"
#include "booking.h"
#include "io.h"

#define RUN_FROM_IDE
#define SHORTEN_SCREENINGS_INFO

#define BLK "\e[0;30m"
#define RED "\e[0;31m"
#define GRN "\e[0;32m"
#define INT32DSTR_LEN 17

struct booking_data {
	uint32_t cinema_id;
	char* cinema_address;
	uint8_t hall;
	char date[DATE_LEN];
	char time[TIME_LEN];
	char film_name[45];
	char price[18];
	char seat_row;
	uint8_t seat_number;
};

const char* title = "*********************************\n\
				   \r*       SCREENINGS BOOKER       *\n\
				   \r*********************************\n";

static int show_cinema(cinema_management_service_t cms, struct booking_data* booking_data);
static int show_screenings(cinema_management_service_t cms, struct booking_data* booking_data);
static int show_available_seats(cinema_management_service_t cms, struct booking_data* booking_data);
static int make_booking(cinema_management_service_t cms, struct booking_data* booking_data);
static int print_cinema_number_header(int cols);
static int print_screen(int cols);

int main(int argc, char** argv) {
#ifdef RUN_FROM_IDE
	putenv("HOST=localhost");
	putenv("DB=cinemadb");
	putenv("PORT=3306");
	putenv("USHER_USERNAME=cliente");
	putenv("USHER_PASSWORD=pippo");
#endif
	bool end = false;
	cinema_management_service_t cms;
	char* username = getenv("USHER_USERNAME");
	char* password = getenv("USHER_PASSWORD");
	struct booking_data booking_data = { 0 };
	try(cms = cinema_management_service_init(username, password), NULL, fail);
	show_cinema(cms, &booking_data);
	cinema_management_service_destroy(cms);
	return EXIT_SUCCESS;
fail:
	return EXIT_FAILURE;
}

static int show_cinema(cinema_management_service_t cms, struct booking_data* booking_data) {
	struct get_all_cinema_response* response;
	ft_table_t* table;
	if (get_all_cinema(cms, &response)) {
		printf("%s", response->error_message);
	}
	clear_screen();
	puts(title);
	table = ft_create_table();
	ft_set_cell_prop(table, 0, FT_ANY_COLUMN, FT_CPROP_ROW_TYPE, FT_ROW_HEADER);
	ft_write_ln(table, "CINEMA", "ADDRESS", "OPENING TIME", "CLOSING TIME");
	for (int i = 0; i < response->num_elements; i++) {
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
	get_input("Select a cinema: ", INT32DSTR_LEN, selected_cinema, false);
	booking_data->cinema_id = atoi(selected_cinema);
	booking_data->cinema_address = response->result[booking_data->cinema_id - 1].address;
	// destroy result somehow
	return show_screenings(cms, booking_data);
}

static int show_screenings(cinema_management_service_t cms, struct booking_data* booking_data) {
	struct get_cinema_screenings_request request = { .cinema_id = booking_data->cinema_id };
	struct get_cinema_screenings_response* response;
	ft_table_t* table;
	if (get_cinema_screenings(cms, request, &response)) {
		printf("%s", response->error_message);
	}
	clear_screen();
	puts(title);
	printf("Selected cinema: %s\n\n", booking_data->cinema_address);
	table = ft_create_table();
	ft_set_cell_prop(table, 0, FT_ANY_COLUMN, FT_CPROP_ROW_TYPE, FT_ROW_HEADER);
#ifdef SHORTEN_SCREENINGS_INFO
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
#else
	ft_write_ln(table, "SCREENING", "DATE", "TIME", "HALL", "FILM", "RUNNING TIME", "FILM STUDIO", "CAST", "PRICE");
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
			response->result[i].film_studio,
			response->result[i].cast,
			response->result[i].price);
	}
#endif
	printf("%s\n", ft_to_string(table));
	ft_destroy_table(table);
	char selected_screening[INT32DSTR_LEN];
	get_input("Select a screening: ", INT32DSTR_LEN, selected_screening, false);
	int i = atoi(selected_screening) - 1;
	booking_data->hall = response->result[i].hall_id;
	strcpy(booking_data->date, response->result[i].date);
	strcpy(booking_data->time, response->result[i].time);
	strcpy(booking_data->film_name, response->result[i].film_name);
	strcpy(booking_data->price, response->result[i].price);
	// destroy result somehow
	return show_available_seats(cms, booking_data);
}

static int show_available_seats(cinema_management_service_t cms, struct booking_data* booking_data) {
	struct get_available_seats_request request = {
		.cinema_id = booking_data->cinema_id,
		.hall_id = booking_data->hall,
	};
	strcpy(request.date, booking_data->date);
	strcpy(request.start_time, booking_data->time);
	struct get_available_seats_response* response;
	ft_table_t* table;
	char selected_row[2];
	char selected_number[INT32DSTR_LEN];
	bool selected_seat_available = false;
	do {
		if (get_available_seats(cms, request, &response)) {
			printf("%s", response->error_message);
		}
		clear_screen();
		puts(title);
		printf("Selected date: %s\n", booking_data->date);
		printf("Selected time: %s\n", booking_data->time);
		printf("Selected cinema: %s\n", booking_data->cinema_address);
		printf("Selected hall: %hhu\n", booking_data->hall);
		printf("Selected film: %s\n", booking_data->film_name);
		printf("Price: %s\n", booking_data->price);
		printf("\n");
		printf("\nLegend:\na - available seat\nr - reserved seat\n\n");
		print_cinema_number_header(response->hall_info_result.num_cols);
		for (int i = 0; i < response->hall_info_result.num_rows; i++) {
			printf(" %c| ", 'A' + i);
			for (int j = 0; j < response->hall_info_result.num_cols; j++) {
				bool available = false;
				for (int k = 0; k < response->num_elements; k++) {
					if (response->seat_result[k].row == ('A' + i) && response->seat_result[k].number == (j + 1)) {
						available = true;
						break;
					}
				}
				printf(" %s ", available ? "a" : "r");
			}
			printf("|\n");
		}
		print_screen(response->hall_info_result.num_cols);
		get_input("Select a row [A,...]: ", sizeof(selected_row), &selected_row, false);
		get_input("Select a number: ", INT32DSTR_LEN, selected_number, false);
		int snumber = atoi(selected_number);
		for (int i = 0; i < response->num_elements; i++) {
			if (response->seat_result[i].row == selected_row[0] && response->seat_result[i].number == snumber) {
				selected_seat_available = true;
				break;
			}
		}
	} while (!selected_seat_available);
	booking_data->seat_row = selected_row[0];
	booking_data->seat_number = atoi(selected_number);
	// destroy result somehow
	return make_booking(cms, booking_data);
}

static int make_booking(cinema_management_service_t cms, struct booking_data* booking_data) {
	char name_on_card[128];
	char card_number[17];
	char expiry_date[DATE_LEN];
	char security_code[4];
	struct book_seat_response* response;
	struct book_seat_request request = {
		.cinema_id = booking_data->cinema_id,
		.hall_id = booking_data->hall,
		.seat_row = booking_data->seat_row,
		.seat_number = booking_data->seat_number
	};
	strcpy(request.date, booking_data->date);
	strcpy(request.start_time, booking_data->time);
	clear_screen();
	puts(title);
	printf("Selected date: %s\n", booking_data->date);
	printf("Selected time: %s\n", booking_data->time);
	printf("Selected cinema: %s\n", booking_data->cinema_address);
	printf("Selected hall: %hhu\n", booking_data->hall);
	printf("Selected film: %s\n", booking_data->film_name);
	printf("Selected seat: %c%hhu\n", booking_data->seat_row, booking_data->seat_number);
	printf("Price: %s\n", booking_data->price);
	printf("\n");
	get_input("Enter name on card: ", sizeof(name_on_card), &name_on_card, false);
	get_input("Enter card number: ", sizeof(card_number), &card_number, false);
	get_input("Enter expiry date [YYYY-MM]: ", sizeof(expiry_date), &expiry_date, false);
	get_input("Enter secuity code (CVV): ", sizeof(security_code), &security_code, true);
	strcpy(request.name_on_card, name_on_card);
	strcpy(request.card_number, card_number);
	strcpy(request.expiry_date, expiry_date);
	strcpy(request.security_code, security_code);
	if (book_seat(cms, request, &response)) {
		printf("%s", response->error_message);
	}
	printf("\nYou booking code is: %d\n", response->result[0].booking_code);
	printf("The reservation code will be required to access the cinema and take advantage of the reservation or to request a refund.\n");
	printf("Save it in a safe place and be careful not to lose it.\n");
	press_anykey();
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
}
