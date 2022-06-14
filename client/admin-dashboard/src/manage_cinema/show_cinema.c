#include <buracchi/common/utilities/try.h>
#include <cms/cms.h>
#include <cms/cinema_management.h>
#include <cliutils/io.h>
#include <fort.h>

#include "../core.h"

static char* get_cinema_table(struct cms_get_all_cinema_response* response);

extern int show_cinema(cms_t cms) {
	struct cms_get_all_cinema_response *response;
	io_clear_screen();
	try(cms_get_all_cinema(cms, &response), 1, fail);
	if (response->error_message) {
		printf("%s", response->error_message);
	}
	else {
		char* cinema_table;
		try(cinema_table = get_cinema_table(response), NULL, fail2);
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
