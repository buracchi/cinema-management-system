#include <inttypes.h>

#include <buracchi/common/utilities/strto.h>
#include <buracchi/common/utilities/try.h>
#include <cms/cms.h>
#include <cms/shift_scheduling.h>
#include <cliutils/io.h>
#include <fort.h>

#include "../core.h"

extern int select_shift(cms_t cms, struct cms_shift_details* selected_shift);
static char* get_shift_table(struct cms_shift_details* shifts, uint64_t num_elements);

extern int show_shift(cms_t cms) {
	struct cms_response response;
	struct cms_shift_details* shifts;
	char* shift_table;
	io_clear_screen();
	response = cms_get_shifts(cms, &shifts);
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
	try(shift_table = get_shift_table(shifts, response.num_elements), NULL, fail);
	puts(title);
	puts(shift_table);
	free(shift_table);
	cms_destroy_response(&response);
	press_anykey();
	return 0;
fail:
	return 1;
}

extern int select_shift(cms_t cms, struct cms_shift_details* selected_shift) {
	struct cms_response response;
	struct cms_shift_details* shifts;
	char* shift_table;
	char input[INT32DSTR_LEN];
	int32_t selected_shift_index;
	bool back = false;
	while (true) {
		io_clear_screen();
		puts(title);
		response = cms_get_shifts(cms, &shifts);
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
		try(shift_table = get_shift_table(shifts, response.num_elements), NULL, fail);
		puts(shift_table);
		free(shift_table);
		get_input("Inserire il numero della proiezione scelta o Q per tornare indietro: ", input, false);
		if (input[0] == 'Q' && input[1] == '\0') {
			back = true;
			break;
		}
		else if (cmn_strto_int32(&selected_shift_index, input, 10) == 0
			&& selected_shift_index > 0
			&& (uint64_t)selected_shift_index <= response.num_elements) {
			memcpy(selected_shift, &(shifts[selected_shift_index - 1]), sizeof * selected_shift);
			break;
		}
		cms_destroy_response(&response);
	}
	cms_destroy_response(&response);
	return back ? 2 : 0;
fail:
	return 1;
}

static char* get_shift_table(struct cms_shift_details* shifts, uint64_t num_elements) {
	char* result;
	ft_table_t* table;
	const char* str_table;
	try(table = ft_create_table(), NULL, fail);
	try(ft_set_cell_prop(table, 0, FT_ANY_COLUMN, FT_CPROP_ROW_TYPE, FT_ROW_HEADER) < 0, true, fail2);
	try(ft_write_ln(table, "TURNO", "GIORNO", "ORA INIZIO", "DURATA", "CINEMA", "MATRICOLA", "NOME", "COGNOME", "RUOLO") < 0, true, fail2);
	for (uint64_t i = 0; i < num_elements; i++) {
		char shift[INT32DSTR_LEN] = { 0 };
		char employee_id[INT32DSTR_LEN] = { 0 };
		char* day = shifts[i].day;
		char* start_time = shifts[i].start_time;
		char* duration = shifts[i].duration;
		char* address = shifts[i].cinema_address;
		char* name = shifts[i].employee_name;
		char* surname = shifts[i].employee_surname;
		char* role = shifts[i].employee_role;
		try(snprintf(shift, INT32DSTR_LEN, "%" PRIu64, i + 1) < 0, true, fail2);
		try(snprintf(employee_id, INT32DSTR_LEN, "%d", shifts[i].employee_id) < 0, true, fail2);
		try(ft_write_ln(table, shift, day, start_time, duration, address, employee_id, name, surname, role) < 0, true, fail2);
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
