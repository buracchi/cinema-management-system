#include <buracchi/common/utilities/try.h>
#include <cms/cms.h>
#include <cms/report.h>
#include <cliutils/io.h>
#include <fort.h>

#include "../core.h"

static char* get_cinema_table(struct cms_get_cinema_without_ushers_response* response);

extern int show_cinema_without_enough_ushers(cms_t cms) {
	struct cms_get_cinema_without_ushers_response *response;
	io_clear_screen();
	puts(title);
	try(cms_get_cinema_without_ushers(cms, &response), 1, fail);
	if (response->error_message) {
		printf("%s", response->error_message);
	}
	else {
		char* cinema_table;
		try(cinema_table = get_cinema_table(response), NULL, fail2);
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

static char* get_cinema_table(struct cms_get_cinema_without_ushers_response* response) {
	char* result;
	ft_table_t* table;
	const char* str_table;
	try(table = ft_create_table(), NULL, fail);
	try(ft_set_cell_prop(table, 0, FT_ANY_COLUMN, FT_CPROP_ROW_TYPE, FT_ROW_HEADER) < 0, true, fail2);
	try(ft_write_ln(table, "CINEMA", "INDIRIZZO", "GIORNO", "DALLE ORE", "ALLE ORE", "NUMERO MASCHERE") < 0, true, fail2);
	for (uint64_t i = 0; i < response->num_elements; i++) {
		char cinema[INT32DSTR_LEN] = { 0 };
		char* address = response->result[i].address;
		char* day = response->result[i].day;
		char* since = response->result[i].since;
		char* until = response->result[i].until;
		char* ushers = response->result[i].ushers;
		try(snprintf(cinema, INT32DSTR_LEN, "%d", response->result[i].cinema) < 0, true, fail2);
		try(ft_write_ln(table, cinema, address, day, since, until, ushers) < 0, true, fail2);
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
