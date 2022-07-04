#include <inttypes.h>

#include <buracchi/common/utilities/strto.h>
#include <buracchi/common/utilities/try.h>
#include <cms/cms.h>
#include <cms/cinema_management.h>
#include <cliutils/io.h>
#include <fort.h>

#include "../core.h"

extern int select_cinema(cms_t cms, struct cms_cinema* selected_cinema);
static char* get_cinema_table(struct cms_cinema* cinema, uint64_t num_elements);

extern int show_cinema(cms_t cms) {
	struct cms_response response;
	struct cms_cinema* cinema;
	char* cinema_table;
	io_clear_screen();
	response = cms_get_all_cinema(cms, &cinema);
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
	try(cinema_table = get_cinema_table(cinema, response.num_elements), NULL, fail);
	puts(title);
	puts(cinema_table);
	free(cinema_table);
	cms_destroy_response(&response);
	press_anykey();
	return 0;
fail:
	cms_destroy_response(&response);
	return 1;
}

extern int select_cinema(cms_t cms, struct cms_cinema* selected_cinema) {
	struct cms_response response;
	struct cms_cinema* cinema;
	char* cinema_table;
	char input[INT32DSTR_LEN];
	int32_t selected_cinema_index;
	bool back = false;
	while (true) {
		io_clear_screen();
		puts(title);
		response = cms_get_all_cinema(cms, &cinema);
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
		try(cinema_table = get_cinema_table(cinema, response.num_elements), NULL, fail);
		puts(cinema_table);
		free(cinema_table);
		get_input("Inserire il numero del cinema scelto o Q per tornare indietro: ", input, false);
		if (input[0] == 'Q' && input[1] == '\0') {
			back = true;
			break;
		}
		else if (cmn_strto_int32(&selected_cinema_index, input, 10) == 0
			&& selected_cinema_index > 0
			&& (uint64_t)selected_cinema_index <= response.num_elements) {
			memcpy(selected_cinema, &(cinema[selected_cinema_index - 1]), sizeof * selected_cinema);
			break;
		}
		cms_destroy_response(&response);
	}
	cms_destroy_response(&response);
	return back ? 2 : 0;
fail:
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
