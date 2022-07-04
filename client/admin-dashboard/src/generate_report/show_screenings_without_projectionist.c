#include <buracchi/common/utilities/try.h>
#include <cms/cms.h>
#include <cms/report.h>
#include <cliutils/io.h>
#include <fort.h>

#include "../core.h"

static char* get_screenings_table(struct cms_screening_without_projectionist* screenings, uint64_t num_elements);

extern int show_screenings_without_projectionist(cms_t cms) {
	struct cms_response response;
	struct cms_screening_without_projectionist* screenings;
	io_clear_screen();
	puts(title);
	response = cms_get_screenings_without_projectionist(cms, &screenings);
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
	char* screenings_table;
	try(screenings_table = get_screenings_table(screenings, response.num_elements), NULL, fail);
	puts(screenings_table);
	free(screenings_table);
	cms_destroy_response(&response);
	press_anykey();
	return 0;
fail:
	return 1;
}

static char* get_screenings_table(struct cms_screening_without_projectionist* screenings, uint64_t num_elements) {
	char* result;
	ft_table_t* table;
	const char* str_table;
	try(table = ft_create_table(), NULL, fail);
	try(ft_set_cell_prop(table, 0, FT_ANY_COLUMN, FT_CPROP_ROW_TYPE, FT_ROW_HEADER) < 0, true, fail2);
	try(ft_write_ln(table, "CINEMA", "SALA", "DATA", "ORA", "PREZZO", "FILM") < 0, true, fail2);
	for (uint64_t i = 0; i < num_elements; i++) {
		char cinema[INT32DSTR_LEN] = { 0 };
		char hall[INT32DSTR_LEN] = { 0 };
		char* date = screenings[i].date;
		char* time = screenings[i].time;
		char* price = screenings[i].price;
		char* film = screenings[i].film_name;
		try(snprintf(cinema, INT32DSTR_LEN, "%d", screenings[i].cinema) < 0, true, fail2);
		try(snprintf(hall, INT32DSTR_LEN, "%d", screenings[i].hall) < 0, true, fail2);
		try(ft_write_ln(table, cinema, hall, date, time, price, film) < 0, true, fail2);
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
