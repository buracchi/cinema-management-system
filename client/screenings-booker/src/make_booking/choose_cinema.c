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

static char* get_cinema_table(struct cms_cinema* cinema, uint64_t num_elements);

extern int choose_cinema(cms_t cms, struct booking_data* booking_data) {
	struct cms_response response;
	struct cms_cinema* cinema;
	char* cinema_table;
	char input[INT32DSTR_LEN];
	int32_t selected_cinema;
	bool back = false;
	while (true) {
		io_clear_screen();
		puts(title);
		response = cms_get_all_cinema(cms, &cinema);
		if (response.fatal_error) {
			goto fail;
		}
		if (response.error_message) {
			printf("%s\n", response.error_message);
			cms_destroy_response(&response);
			press_anykey();
			return 0;
		}
		try(cinema_table = get_cinema_table(cinema, response.num_elements), NULL, fail);
		puts(cinema_table);
		free(cinema_table);
		get_input("Inserire il numero del cinema scelto o Q per tornare indietro: ", input, false);
		if (input[0] == 'Q' && input[1] == '\0') {
			back = true;
			break;
		}
		else if (cmn_strto_int32(&selected_cinema, input, 10) == 0
			&& selected_cinema > 0
			&& (uint64_t)selected_cinema <= response.num_elements) {
			booking_data->cinema_id = cinema[selected_cinema - 1].id;
			strcpy(booking_data->cinema_address, cinema[selected_cinema - 1].address);
			break;
		}
		cms_destroy_response(&response);
	}
	cms_destroy_response(&response);
	return back ? 0 : choose_screening(cms, booking_data);
fail:
	fprintf(stderr, "%s\n", response.error_message ? response.error_message : cms_get_error_message(cms));
	cms_destroy_response(&response);
	return 1;
}

static char* get_cinema_table(struct cms_cinema* cinema, uint64_t num_elements) {
	char* result;
	ft_table_t* table;
	const char* str_table;
	try(table = ft_create_table(), NULL, fail);
	try(ft_set_cell_prop(table, 0, FT_ANY_COLUMN, FT_CPROP_ROW_TYPE, FT_ROW_HEADER) < 0, true, fail2);
	try(ft_write_ln(table, "CINEMA", "INDIRIZZO", "APERTURA", "CHIUSURA") < 0, true, fail2);
	for (uint64_t i = 0; i < num_elements; i++) {
		char id[INT32DSTR_LEN] = { 0 };
		char* address = cinema[i].address;
		char* opening = cinema[i].opening_time;
		char* closing = cinema[i].closing_time;
		try(snprintf(id, INT32DSTR_LEN, "%" PRIu64, i + 1) < 0, true, fail2);
		try(ft_write_ln(table, id, address, opening, closing) < 0, true, fail2);
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
