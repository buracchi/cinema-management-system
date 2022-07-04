#include <inttypes.h>

#include <buracchi/common/utilities/strto.h>
#include <buracchi/common/utilities/try.h>
#include <cms/cms.h>
#include <cms/projections_scheduling.h>
#include <cliutils/io.h>
#include <fort.h>

#include "../core.h"

extern int select_projection(cms_t cms, struct cms_projection_details* selected_projection);
static char* get_projections_table(struct cms_projection_details* projections, uint64_t num_elements);

extern int show_projections(cms_t cms) {
	struct cms_response response;
	struct cms_projection_details* projections;
	char* projection_table;
	io_clear_screen();
	response = cms_get_projections(cms, &projections);
	if (response.fatal_error) {
		fprintf(stderr, "%s\n", response.error_message ? response.error_message : cms_get_error_message(cms));
		cms_destroy_response(&response);
		return 1;
	}
	if (response.error_message) {
		printf("%s\n", response.error_message);
		cms_destroy_response(&response);
		press_anykey();
		return 0;
	}
	try(projection_table = get_projections_table(projections, response.num_elements), NULL, fail);
	puts(title);
	puts(projection_table);
	free(projection_table);
	cms_destroy_response(&response);
	press_anykey();
	return 0;
fail:
	return 1;
}

extern int select_projection(cms_t cms, struct cms_projection_details* selected_projection) {
	struct cms_response response;
	struct cms_projection_details* projections;
	char* projection_table;
	char input[INT32DSTR_LEN];
	int32_t selected_projection_index;
	bool back = false;
	while (true) {
		io_clear_screen();
		puts(title);
		response = cms_get_projections(cms, &projections);
		if (response.fatal_error) {
			fprintf(stderr, "%s\n", response.error_message ? response.error_message : cms_get_error_message(cms));
			cms_destroy_response(&response);
			return 1;
		}
		if (response.error_message) {
			printf("%s\n", response.error_message);
			cms_destroy_response(&response);
			press_anykey();
			return 2;
		}
		try(projection_table = get_projections_table(projections, response.num_elements), NULL, fail);
		puts(projection_table);
		free(projection_table);
		get_input("Inserire il numero della proiezione scelta o Q per tornare indietro: ", input, false);
		if (input[0] == 'Q' && input[1] == '\0') {
			back = true;
			break;
		}
		else if (cmn_strto_int32(&selected_projection_index, input, 10) == 0
			&& selected_projection_index > 0
			&& (uint64_t)selected_projection_index <= response.num_elements) {
			memcpy(selected_projection, &(projections[selected_projection_index - 1]), sizeof * selected_projection);
			break;
		}
		cms_destroy_response(&response);
	}
	cms_destroy_response(&response);
	return back ? 2 : 0;
fail:
	return 1;
}

static char* get_projections_table(struct cms_projection_details* projections, uint64_t num_elements) {
	char* result;
	ft_table_t* table;
	const char* str_table;
	try(table = ft_create_table(), NULL, fail);
	try(ft_set_cell_prop(table, 0, FT_ANY_COLUMN, FT_CPROP_ROW_TYPE, FT_ROW_HEADER) < 0, true, fail2);
	try(ft_write_ln(table, "PROIEZIONE", "DATA", "CINEMA", "SALA", "FILM", "PREZZO", "INIZIO", "DURATA", "MATRICOLA", "NOME", "COGNOME") < 0, true, fail2);
	for (uint64_t i = 0; i < num_elements; i++) {
		char projection[INT32DSTR_LEN] = { 0 };
		char hall_number[INT32DSTR_LEN] = { 0 };
		char projectionist_id[INT32DSTR_LEN] = { 0 };
		char* date = projections[i].date;
		char* cinema_address = projections[i].cinema_address;
		char* film_name = projections[i].film_name;
		char* price = projections[i].price;
		char* start_time = projections[i].start_time;
		char* duration = projections[i].duration;
		char* projectionist_name = projections[i].projectionist_name;
		char* projectionist_surname = projections[i].projectionist_surname;
		try(snprintf(projection, INT32DSTR_LEN, "%" PRIu64, i + 1) < 0, true, fail2);
		try(snprintf(hall_number, INT32DSTR_LEN, "%d", projections[i].hall_number) < 0, true, fail2);
		if (projections[i].projectionist_id) {
			try(snprintf(projectionist_id, INT32DSTR_LEN, "%d", projections[i].projectionist_id) < 0, true, fail2);
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
