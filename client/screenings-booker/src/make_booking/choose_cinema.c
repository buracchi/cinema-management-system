#include "make_booking.h"

#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>

#include <buracchi/common/utilities/utilities.h>
#include <buracchi/common/utilities/try.h>
#include <fort.h>
#include <cms/booking.h>
#include <cliutils/io.h>
#include <cliutils/strto.h>

#include "../core.h"

static char* get_cinema_table(struct cms_get_all_cinema_response* response);

extern int choose_cinema(cms_t cms, struct booking_data* booking_data) {
	struct cms_get_all_cinema_response* response;
	char* cinema_table;
	char input[INT32DSTR_LEN];
	int32_t selected_cinema;
	bool back = false;
	while (true) {
		io_clear_screen();
		try(cms_get_all_cinema(cms, &response), 1, fail);
		if (response->error_message) {
			printf("%s", response->error_message);
			cms_destroy_response((struct cms_response*)response);
			press_anykey();
			return 0;
		}
		try(cinema_table = get_cinema_table(response), NULL, fail2);
		puts(title);
		puts(cinema_table);
		free(cinema_table);
		get_input("Inserire il numero del cinema scelto o Q per tornare indietro: ", input, false);
		if ((input[0] == 'Q' || input[0] == 'q') && input[1] == '\0') {
			back = true;
			break;
		}
		else if (strtoint32(&selected_cinema, input, 10) == STRTO_SUCCESS
			&& selected_cinema > 0
			&& (uint64_t)selected_cinema <= response->num_elements) {
			booking_data->cinema_id = response->result[selected_cinema - 1].id;
			strcpy(booking_data->cinema_address, response->result[selected_cinema - 1].address);
			break;
		}
		cms_destroy_response((struct cms_response*)response);
	}
	cms_destroy_response((struct cms_response*)response);
	return back ? 0 : choose_screening(cms, booking_data);
fail2:
	cms_destroy_response((struct cms_response*)response);
fail:
	return 1;
}

static char* get_cinema_table(struct cms_get_all_cinema_response* response) {
	char* result;
	ft_table_t* table;
	const char* str_table;
	try(table = ft_create_table(), NULL, fail);
	try(ft_set_cell_prop(table, 0, FT_ANY_COLUMN, FT_CPROP_ROW_TYPE, FT_ROW_HEADER) < 0, true, fail2);
	try(ft_write_ln(table, "CINEMA", "INDIRIZZO", "APERTURA", "CHIUSURA") < 0, true, fail2);
	for (uint64_t i = 0; i < response->num_elements; i++) {
		char id[INT32DSTR_LEN] = { 0 };
		char* address = response->result[i].address;
		char* opening = response->result[i].opening_time;
		char* closing = response->result[i].closing_time;
		try(snprintf(id, INT32DSTR_LEN, "%llu", i + 1) < 0, true, fail2);
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
