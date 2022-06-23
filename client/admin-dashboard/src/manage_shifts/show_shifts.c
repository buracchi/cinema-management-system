#include <buracchi/common/utilities/strto.h>
#include <buracchi/common/utilities/try.h>
#include <cms/cms.h>
#include <cms/shift_scheduling.h>
#include <cliutils/io.h>
#include <fort.h>

#include "../core.h"

extern int select_shift(cms_t cms, struct cms_shift* shift);
static char* get_shift_table(struct cms_get_shifts_response* response);

extern int show_shift(cms_t cms) {
	struct cms_get_shifts_response* response = NULL;
	io_clear_screen();
	try(cms_get_shifts(cms, &response), 1, fail);
	if (response->error_message) {
		printf("%s\n", response->error_message);
	}
	else {
		char* shift_table;
		try(shift_table = get_shift_table(response), NULL, fail);
		puts(title);
		puts(shift_table);
		free(shift_table);
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

extern int select_shift(cms_t cms, struct cms_shift* shift) {
	struct cms_get_shifts_response* response = NULL;
	char* shift_table;
	char input[INT32DSTR_LEN];
	int32_t selected_shift;
	bool back = false;
	while (true) {
		io_clear_screen();
		puts(title);
		try(cms_get_shifts(cms, &response), 1, fail);
		if (response->error_message) {
			printf("%s\n", response->error_message);
			cms_destroy_response((struct cms_response*)response);
			press_anykey();
			return 2;
		}
		try(shift_table = get_shift_table(response), NULL, fail);
		puts(shift_table);
		free(shift_table);
		get_input("Inserire il numero della proiezione scelta o Q per tornare indietro: ", input, false);
		if (input[0] == 'Q' && input[1] == '\0') {
			back = true;
			break;
		}
		else if (cmn_strto_int32(&selected_shift, input, 10) == 0
		         && selected_shift > 0
		         && (uint64_t)selected_shift <= response->num_elements) {
			memcpy(shift, &(response->result[selected_shift - 1]), sizeof * shift);
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

static char* get_shift_table(struct cms_get_shifts_response* response) {
	char* result;
	ft_table_t* table;
	const char* str_table;
	try(table = ft_create_table(), NULL, fail);
	try(ft_set_cell_prop(table, 0, FT_ANY_COLUMN, FT_CPROP_ROW_TYPE, FT_ROW_HEADER) < 0, true, fail2);
	try(ft_write_ln(table, "TURNO", "GIORNO", "ORA INIZIO", "DURATA", "CINEMA", "MATRICOLA", "NOME", "COGNOME", "RUOLO") < 0, true, fail2);
	for (uint64_t i = 0; i < response->num_elements; i++) {
		char shift[INT32DSTR_LEN] = {0 };
		char employee_id[INT32DSTR_LEN] = {0 };
		char* day = response->result[i].day;
		char* start_time = response->result[i].start_time;
		char* duration = response->result[i].duration;
		char* address = response->result[i].cinema_address;
		char* name = response->result[i].employee_name;
		char* surname = response->result[i].employee_surname;
		char* role = response->result[i].employee_role;
		try(snprintf(shift, INT32DSTR_LEN, "%llu", i + 1) < 0, true, fail2);
		try(snprintf(employee_id, INT32DSTR_LEN, "%d", response->result[i].employee_id) < 0, true, fail2);
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
