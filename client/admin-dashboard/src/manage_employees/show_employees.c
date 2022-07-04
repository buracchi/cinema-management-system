#include <buracchi/common/utilities/strto.h>
#include <buracchi/common/utilities/try.h>
#include <cms/cms.h>
#include <cms/employee_management.h>
#include <cliutils/io.h>
#include <fort.h>

#include "../core.h"

extern int select_employee(cms_t cms, struct cms_employee* selected_employee);
static char* get_employees_table(struct cms_employee* employees, uint64_t num_elements);

extern int show_employees(cms_t cms) {
	struct cms_response response;
	struct cms_employee* employees;
	io_clear_screen();
	response = cms_get_employees(cms, &employees);
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
	char* cinema_table;
	try(cinema_table = get_employees_table(employees, response.num_elements), NULL, fail);
	puts(title);
	puts(cinema_table);
	free(cinema_table);
	cms_destroy_response(&response);
	press_anykey();
	return 0;
fail:
	return 1;
}

extern int select_employee(cms_t cms, struct cms_employee* selected_employee) {
	struct cms_response response;
	struct cms_employee* employees;
	char* employees_table;
	char input[INT32DSTR_LEN];
	int32_t selected_employee_index;
	bool back = false;
	while (true) {
		bool valid_input = false;
		io_clear_screen();
		puts(title);
		response = cms_get_employees(cms, &employees);
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
		try(employees_table = get_employees_table(employees, response.num_elements), NULL, fail);
		puts(employees_table);
		free(employees_table);
		get_input("Inserire il numero del selected_employee scelto o Q per tornare indietro: ", input, false);
		if (input[0] == 'Q' && input[1] == '\0') {
			back = true;
			break;
		}
		else if (cmn_strto_int32(&selected_employee_index, input, 10) == 0) {
			for (uint64_t i = 0; i < response.num_elements; i++) {
				if (selected_employee_index == employees[i].id) {
					memcpy(selected_employee, &(employees[i]), sizeof * selected_employee);
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

static char* get_employees_table(struct cms_employee* employees, uint64_t num_elements) {
	char* result;
	ft_table_t* table;
	const char* str_table;
	try(table = ft_create_table(), NULL, fail);
	try(ft_set_cell_prop(table, 0, FT_ANY_COLUMN, FT_CPROP_ROW_TYPE, FT_ROW_HEADER) < 0, true, fail2);
	try(ft_write_ln(table, "MATRICOLA", "NOME", "COGNOME", "RUOLO") < 0, true, fail2);
	for (uint64_t i = 0; i < num_elements; i++) {
		char id[INT32DSTR_LEN] = { 0 };
		char* address = employees[i].name;
		char* opening = employees[i].surname;
		char* closing = employees[i].role;
		try(snprintf(id, INT32DSTR_LEN, "%d", employees[i].id) < 0, true, fail2);
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
