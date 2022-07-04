#include "make_booking.h"

#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <inttypes.h>

#include <buracchi/common/utilities/utilities.h>
#include <buracchi/common/utilities/strto.h>
#include <buracchi/common/utilities/try.h>
#include <fort.h>
#include <cms/booking.h>
#include <cliutils/io.h>

#include "../core.h"

static char* get_screenings_table(struct cms_screening_details* screenings, uint64_t num_elements);

extern int choose_screening(cms_t cms, struct booking_data* booking_data) {
	struct cms_response response;
	struct cms_screening_details* screenings;
	char* screenings_table;
	char input[INT32DSTR_LEN];
	int32_t selected_screening;
	bool back = false;
	while (true) {
		io_clear_screen();
		puts(title);
		response = cms_get_cinema_screenings(cms, booking_data->cinema_id, &screenings);
		if (response.fatal_error) {
			goto fail;
		}
		if (response.error_message) {
			printf("%s\n", response.error_message);
			cms_destroy_response(&response);
			press_anykey();
			return 0;
		}
		try(screenings_table = get_screenings_table(screenings, response.num_elements), NULL, fail);
		printf("Cinema selezionato: %s\n\n", booking_data->cinema_address);
		puts(screenings_table);
		free(screenings_table);
		get_input("Inserire il numero della proiezione scelta o Q per tornare indietro: ", input, false);
		if (input[0] == 'Q' && input[1] == '\0') {
			back = true;
			break;
		}
		else if (cmn_strto_int32(&selected_screening, input, 10) == 0
			&& selected_screening > 0
			&& (uint64_t)selected_screening <= response.num_elements) {
			booking_data->hall = screenings[selected_screening - 1].hall_number;
			strcpy(booking_data->date, screenings[selected_screening - 1].date);
			strcpy(booking_data->time, screenings[selected_screening - 1].start_time);
			strcpy(booking_data->film_name, screenings[selected_screening - 1].film_name);
			strcpy(booking_data->price, screenings[selected_screening - 1].price);
			break;
		}
		cms_destroy_response(&response);
	}
	cms_destroy_response(&response);
	return back ? choose_cinema(cms, booking_data) : choose_seat(cms, booking_data);
fail:
	fprintf(stderr, "%s\n", response.error_message ? response.error_message : cms_get_error_message(cms));
	cms_destroy_response(&response);
	return 1;
}

static char* get_screenings_table(struct cms_screening_details* screenings, uint64_t num_elements) {
	char* result;
	ft_table_t* table;
	const char* str_table;
	try(table = ft_create_table(), NULL, fail);
	try(ft_set_cell_prop(table, 0, FT_ANY_COLUMN, FT_CPROP_ROW_TYPE, FT_ROW_HEADER) < 0, true, fail2);
	try(ft_write_ln(table, "PROIEZIONE", "DATA", "INIZIO", "SALA", "FILM", "DURATA", "PREZZO") < 0, true, fail2);
	for (uint64_t i = 0; i < num_elements; i++) {
		char screening_id[UINT64DSTR_LEN] = { 0 };
		char hall_number[INT32DSTR_LEN] = { 0 };
		char* date = screenings[i].date;
		char* time = screenings[i].start_time;
		char* film_name = screenings[i].film_name;
		char* running_time = screenings[i].running_time;
		char* price = screenings[i].price;
		try(snprintf(screening_id, UINT64DSTR_LEN, "%" PRIu64, i + 1) < 0, true, fail2);
		try(snprintf(hall_number, INT32DSTR_LEN, "%d", screenings[i].hall_number) < 0, true, fail2);
		try(ft_write_ln(table, screening_id, date, time, hall_number, film_name, running_time, price) < 0, true, fail2);
	}
	try(str_table = ft_to_string((const ft_table_t*)table), NULL, fail2);
	try(result = malloc(strlen(str_table) + 1), NULL, fail2);
	strcpy(result, str_table);
	ft_destroy_table(table);
	return result;
fail2:
	ft_destroy_table(table);
fail:
	return NULL;
}
