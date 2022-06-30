#include <buracchi/common/utilities/strto.h>
#include <buracchi/common/utilities/try.h>
#include <cms/cms.h>
#include <cms/projections_scheduling.h>
#include <cliutils/io.h>
#include <fort.h>

#include "../core.h"

extern int select_projection(cms_t cms, struct cms_projection_details *projection_details);
static char* get_projections_table(struct cms_get_projections_response* response);

extern int show_projections(cms_t cms) {
	struct cms_get_projections_response* response = NULL;
	io_clear_screen();
	try(cms_get_projections(cms, &response), 1, fail);
	if (response->error_message) {
		printf("%s\n", response->error_message);
	}
	else {
		char* projection_table;
		try(projection_table = get_projections_table(response), NULL, fail);
		puts(title);
		puts(projection_table);
		free(projection_table);
	}
	cms_destroy_response((struct cms_response*)response);
	press_anykey();
	return 0;
fail:
	if (response) {
		if (response->error_message) {
			fprintf(stderr, "%s\n", response->error_message);
		}
		cms_destroy_response((struct cms_response*)response);
	}
	return 1;
}

extern int select_projection(cms_t cms, struct cms_projection_details *projection_details) {
	struct cms_get_projections_response* response = NULL;
	char* projection_table;
	char input[INT32DSTR_LEN];
	int32_t selected_projection;
	bool back = false;
	while (true) {
		io_clear_screen();
		puts(title);
		try(cms_get_projections(cms, &response), 1, fail);
		if (response->error_message) {
			printf("%s\n", response->error_message);
			cms_destroy_response((struct cms_response*)response);
			press_anykey();
			return 2;
		}
		try(projection_table = get_projections_table(response), NULL, fail);
		puts(projection_table);
		free(projection_table);
		get_input("Inserire il numero della proiezione scelta o Q per tornare indietro: ", input, false);
		if (input[0] == 'Q' && input[1] == '\0') {
			back = true;
			break;
		}
		else if (cmn_strto_int32(&selected_projection, input, 10) == 0
		         && selected_projection > 0
		         && (uint64_t)selected_projection <= response->num_elements) {
			memcpy(projection_details, &(response->result[selected_projection - 1]), sizeof * projection_details);
			break;
		}
		cms_destroy_response((struct cms_response*)response);
	}
	cms_destroy_response((struct cms_response*)response);
	return back ? 2 : 0;
fail:
	if (response) {
		if (response->error_message) {
			fprintf(stderr, "%s\n", response->error_message);
		}
		cms_destroy_response((struct cms_response*)response);
	}
	return 1;
}

static char* get_projections_table(struct cms_get_projections_response* response) {
	char* result;
	ft_table_t* table;
	const char* str_table;
	try(table = ft_create_table(), NULL, fail);
	try(ft_set_cell_prop(table, 0, FT_ANY_COLUMN, FT_CPROP_ROW_TYPE, FT_ROW_HEADER) < 0, true, fail2);
	try(ft_write_ln(table, "PROIEZIONE", "DATA", "CINEMA", "SALA", "FILM", "PREZZO", "INIZIO", "DURATA", "MATRICOLA", "NOME", "COGNOME") < 0, true, fail2);
	for (uint64_t i = 0; i < response->num_elements; i++) {
		char projection[INT32DSTR_LEN] = { 0 };
		char hall_number[INT32DSTR_LEN] = { 0 };
		char projectionist_id[INT32DSTR_LEN] = { 0 };
		char* date = response->result[i].date;
		char* cinema_address = response->result[i].cinema_address;
		char* film_name = response->result[i].film_name;
		char* price = response->result[i].price;
		char* start_time = response->result[i].start_time;
		char* duration = response->result[i].duration;
		char* projectionist_name = response->result[i].projectionist_name;
		char* projectionist_surname = response->result[i].projectionist_surname;
		try(snprintf(projection, INT32DSTR_LEN, "%llu", i + 1) < 0, true, fail2);
		try(snprintf(hall_number, INT32DSTR_LEN, "%d", response->result[i].hall_number) < 0, true, fail2);
		if (response->result[i].projectionist_id) {
			try(snprintf(projectionist_id, INT32DSTR_LEN, "%d", response->result[i].projectionist_id) < 0, true, fail2);
		}
		try(ft_write_ln(table, projection, date, cinema_address, hall_number, film_name, price, start_time, duration, projectionist_id, projectionist_name, projectionist_surname) < 0, true, fail2);
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
