#include <buracchi/common/utilities/try.h>
#include <cms/cms.h>
#include <cms/screenings_scheduling.h>
#include <cliutils/io.h>
#include <cliutils/strto.h>
#include <fort.h>

#include "../core.h"

static char* get_projectionists_table(struct cms_get_available_projectionists_response* response);

extern int select_projectionist(cms_t cms, struct cms_get_available_projectionists_request request, struct cms_available_projectionist* projectionist) {
	struct cms_get_available_projectionists_response* response;
	char* projectionists_table;
	char input[INT32DSTR_LEN];
	int32_t selected_movie;
	bool back = false;
	while (true) {
		io_clear_screen();
		puts(title);
		try(cms_get_available_projectionists(cms, request, &response), 1, fail);
		if (response->error_message) {
			printf("%s", response->error_message);
			cms_destroy_response((struct cms_response*)response);
			press_anykey();
			return 2;
		}
		try(projectionists_table = get_projectionists_table(response), NULL, fail2);
		puts(projectionists_table);
		free(projectionists_table);
		get_input("Inserire il numero del film scelto o Q per tornare indietro: ", input, false);
		if ((input[0] == 'Q' || input[0] == 'q') && input[1] == '\0') {
			back = true;
			break;
		}
		else if (strtoint32(&selected_movie, input, 10) == STRTO_SUCCESS
		         && selected_movie > 0
		         && (uint64_t)selected_movie <= response->num_elements) {
			memcpy(projectionist, &(response->result[selected_movie - 1]), sizeof * projectionist);
			break;
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

static char* get_projectionists_table(struct cms_get_available_projectionists_response* response) {
	char* result;
	ft_table_t* table;
	const char* str_table;
	try(table = ft_create_table(), NULL, fail);
	try(ft_set_cell_prop(table, 0, FT_ANY_COLUMN, FT_CPROP_ROW_TYPE, FT_ROW_HEADER) < 0, true, fail2);
	try(ft_write_ln(table, "PROIEZIONISTA", "NOME", "COGNOME") < 0, true, fail2);
	for (uint64_t i = 0; i < response->num_elements; i++) {
		char index[INT32DSTR_LEN] = { 0 };
		char* name = response->result[i].name;
		char* surname = response->result[i].surname;
		try(snprintf(index, INT32DSTR_LEN, "%llu", i + 1) < 0, true, fail2);
		try(ft_write_ln(table, index, name, surname) < 0, true, fail2);
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