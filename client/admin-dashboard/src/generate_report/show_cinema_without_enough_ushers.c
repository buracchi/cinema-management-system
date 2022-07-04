#include <buracchi/common/utilities/try.h>
#include <cms/cms.h>
#include <cms/report.h>
#include <cliutils/io.h>
#include <fort.h>

#include "../core.h"

static char* get_cinema_table(struct cms_cinema_without_ushers* cms_cinema_without_ushers, uint64_t num_elements);

extern int show_cinema_without_enough_ushers(cms_t cms) {
	struct cms_response response;
	struct cms_cinema_without_ushers* cinema_without_ushers;
	char* cinema_table;
	io_clear_screen();
	puts(title);
	response = cms_get_cinema_without_ushers(cms, &cinema_without_ushers);
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
	try(cinema_table = get_cinema_table(cinema_without_ushers, response.num_elements), NULL, fail);
	puts(cinema_table);
	free(cinema_table);
	cms_destroy_response(&response);
	press_anykey();
	return 0;
fail:
	return 1;
}

static char* get_cinema_table(struct cms_cinema_without_ushers* cms_cinema_without_ushers, uint64_t num_elements) {
	char* result;
	ft_table_t* table;
	const char* str_table;
	try(table = ft_create_table(), NULL, fail);
	try(ft_set_cell_prop(table, 0, FT_ANY_COLUMN, FT_CPROP_ROW_TYPE, FT_ROW_HEADER) < 0, true, fail2);
	try(ft_write_ln(table, "CINEMA", "INDIRIZZO", "GIORNO", "DALLE ORE", "ALLE ORE", "NUMERO MASCHERE") < 0, true, fail2);
	for (uint64_t i = 0; i < num_elements; i++) {
		char cinema[INT32DSTR_LEN] = { 0 };
		char* address = cms_cinema_without_ushers[i].address;
		char* day = cms_cinema_without_ushers[i].day;
		char* since = cms_cinema_without_ushers[i].since;
		char* until = cms_cinema_without_ushers[i].until;
		char* ushers = cms_cinema_without_ushers[i].ushers;
		try(snprintf(cinema, INT32DSTR_LEN, "%d", cms_cinema_without_ushers[i].cinema) < 0, true, fail2);
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
