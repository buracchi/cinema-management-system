#include <inttypes.h>

#include <buracchi/common/utilities/strto.h>
#include <buracchi/common/utilities/try.h>
#include <cms/cms.h>
#include <cms/cinema_management.h>
#include <cliutils/io.h>
#include <fort.h>

#include "../core.h"

extern int select_cinema(cms_t cms, struct cms_cinema* cinema);
extern int select_hall(cms_t cms, int32_t cinema_id, struct cms_hall* selected_hall);
static char* get_halls_table(struct cms_hall* halls, uint64_t num_elements);

extern int show_halls(cms_t cms) {
	struct cms_response response;
	struct cms_cinema cinema = { 0 };
	struct cms_hall* halls;
	char* halls_table;
	switch (select_cinema(cms, &cinema)) {
	case 1:
		return 1;
	case 2:
		return 0;
	}
	io_clear_screen();
	puts(title);
	response = cms_get_cinema_halls(cms, cinema.id, &halls);
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
	try(halls_table = get_halls_table(halls, response.num_elements), NULL, fail);
	puts(halls_table);
	free(halls_table);
	cms_destroy_response(&response);
	press_anykey();
	return 0;
fail:
	return 1;
}

extern int select_hall(cms_t cms, int32_t cinema_id, struct cms_hall* selected_hall) {
	struct cms_response response;
	struct cms_hall* halls;
	char* hall_table;
	char input[INT32DSTR_LEN];
	int32_t selected_hall_index;
	bool back = false;
	while (true) {
		bool valid_input = false;
		io_clear_screen();
		puts(title);
		response = cms_get_cinema_halls(cms, cinema_id, &halls);
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
		try(hall_table = get_halls_table(halls, response.num_elements), NULL, fail);
		puts(hall_table);
		free(hall_table);
		get_input("Inserire il numero della sala scelta o Q per tornare indietro: ", input, false);
		if (input[0] == 'Q' && input[1] == '\0') {
			back = true;
			break;
		}
		else if (cmn_strto_int32(&selected_hall_index, input, 10) == 0) {
			for (uint64_t i = 0; i < response.num_elements; i++) {
				if (selected_hall_index == halls[i].id) {
					memcpy(selected_hall, &(halls[i]), sizeof * selected_hall);
					valid_input = true;
					break;
				}
			}
			if (valid_input) {
				break;
			}
		}
		cms_destroy_response(&response);
	}
	cms_destroy_response(&response);
	return back ? 2 : 0;
fail:
	return 1;
}

static char* get_halls_table(struct cms_hall* halls, uint64_t num_elements) {
	char* result;
	ft_table_t* table;
	const char* str_table;
	try(table = ft_create_table(), NULL, fail);
	try(ft_set_cell_prop(table, 0, FT_ANY_COLUMN, FT_CPROP_ROW_TYPE, FT_ROW_HEADER) < 0, true, fail2);
	try(ft_write_ln(table, "SALA", "FILE", "COLONNE") < 0, true, fail2);
	for (uint64_t i = 0; i < num_elements; i++) {
		char id[INT32DSTR_LEN] = { 0 };
		char rows[UINT64DSTR_LEN] = { 0 };
		char cols[UINT64DSTR_LEN] = { 0 };
		try(snprintf(id, INT32DSTR_LEN, "%d", halls[i].id) < 0, true, fail2);
		try(snprintf(rows, UINT64DSTR_LEN, "%" PRIu64, halls[i].num_rows) < 0, true, fail2);
		try(snprintf(cols, UINT64DSTR_LEN, "%" PRIu64, halls[i].num_cols) < 0, true, fail2);
		try(ft_write_ln(table, id, rows, cols) < 0, true, fail2);
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
