#include <buracchi/common/utilities/try.h>
#include <cms/cms.h>
#include <cms/employee_management.h>
#include <cliutils/io.h>
#include <cliutils/strto.h>
#include <fort.h>

#include "../core.h"

extern int select_employee(cms_t cms, struct cms_employees* employee);
static char* get_employees_table(struct cms_get_employees_response* response);

extern int show_employees(cms_t cms) {
	struct cms_get_employees_response* response;
	io_clear_screen();
	try(cms_get_employees(cms, &response), 1, fail);
	if (response->error_message) {
		printf("%s", response->error_message);
	}
	else {
		char* cinema_table;
		try(cinema_table = get_employees_table(response), NULL, fail2);
		puts(title);
		puts(cinema_table);
		free(cinema_table);
	}
	cms_destroy_response((struct cms_response*)response);
	press_anykey();
	return 0;
fail2:
	cms_destroy_response((struct cms_response*)response);
fail:
	return 1;
}

extern int select_employee(cms_t cms, struct cms_employees* employee) {
	struct cms_get_employees_response* response;
	char* employees_table;
	char input[INT32DSTR_LEN];
	int32_t selected_employee;
	bool back = false;
	while (true) {
		bool valid_input = false;
		io_clear_screen();
		puts(title);
		try(cms_get_employees(cms, &response), 1, fail);
		if (response->error_message) {
			printf("%s", response->error_message);
			cms_destroy_response((struct cms_response*)response);
			press_anykey();
			return 2;
		}
		try(employees_table = get_employees_table(response), NULL, fail2);
		puts(employees_table);
		free(employees_table);
		get_input("Inserire il numero del employee scelto o Q per tornare indietro: ", input, false);
		if ((input[0] == 'Q' || input[0] == 'q') && input[1] == '\0') {
			back = true;
			break;
		}
		else if (strtoint32(&selected_employee, input, 10) == STRTO_SUCCESS) {
			for (uint64_t i = 0; i < response->num_elements; i++) {
				if (selected_employee == response->result[i].id) {
					memcpy(employee, &(response->result[i]), sizeof * employee);
					valid_input = true;
					break;
				}
			}
			if (valid_input) {
				break;
			}
		}
		cms_destroy_response((struct cms_response*)response);
	}
	cms_destroy_response((struct cms_response*)response);
	return back ? 2 : 0;
fail2:
	cms_destroy_response((struct cms_response*)response);
fail:
	return 1;
}

static char* get_employees_table(struct cms_get_employees_response* response) {
	char* result;
	ft_table_t* table;
	const char* str_table;
	try(table = ft_create_table(), NULL, fail);
	try(ft_set_cell_prop(table, 0, FT_ANY_COLUMN, FT_CPROP_ROW_TYPE, FT_ROW_HEADER) < 0, true, fail2);
	try(ft_write_ln(table, "MATRICOLA", "NOME", "COGNOME", "RUOLO") < 0, true, fail2);
	for (uint64_t i = 0; i < response->num_elements; i++) {
		char id[INT32DSTR_LEN] = { 0 };
		char* address = response->result[i].name;
		char* opening = response->result[i].surname;
		char* closing = response->result[i].role;
		try(snprintf(id, INT32DSTR_LEN, "%d", response->result[i].id) < 0, true, fail2);
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
