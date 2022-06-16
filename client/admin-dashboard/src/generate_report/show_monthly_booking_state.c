#include <buracchi/common/utilities/try.h>
#include <cms/cms.h>
#include <cms/report.h>
#include <cliutils/io.h>
#include <fort.h>

#include "../core.h"

static char* get_booking_table(struct cms_get_reservations_status_response* response);

extern int show_monthly_booking_state(cms_t cms) {
	struct cms_get_reservations_status_response* response = NULL;
	io_clear_screen();
	puts(title);
	try(cms_get_reservations_status(cms, &response), 1, fail);
	if (response->error_message) {
		printf("%s", response->error_message);
	}
	else {
		char* booking_table;
		try(booking_table = get_booking_table(response), NULL, fail);
		puts(booking_table);
		free(booking_table);
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

static char* get_booking_table(struct cms_get_reservations_status_response* response) {
	char* result;
	ft_table_t* table;
	const char* str_table;
	try(table = ft_create_table(), NULL, fail);
	try(ft_set_cell_prop(table, 0, FT_ANY_COLUMN, FT_CPROP_ROW_TYPE, FT_ROW_HEADER) < 0, true, fail2);
	try(ft_write_ln(table, "CINEMA", "INDIRIZZO", "SALA", "CONFERMATE", "ANNULLATE", "VALIDATE", "SCADUTE") < 0, true, fail2);
	for (uint64_t i = 0; i < response->num_elements; i++) {
		char cinema[INT32DSTR_LEN] = { 0 };
		char hall[INT32DSTR_LEN] = { 0 };
		char* address = response->result[i].address;
		char* confirmed = response->result[i].confirmed;
		char* cancelled = response->result[i].cancelled;
		char* validated = response->result[i].validated;
		char* expired = response->result[i].expired;
		try(snprintf(cinema, INT32DSTR_LEN, "%d", response->result[i].cinema) < 0, true, fail2);
		try(snprintf(hall, INT32DSTR_LEN, "%d", response->result[i].hall) < 0, true, fail2);
		try(ft_write_ln(table, cinema, address, hall, confirmed, cancelled, validated, expired) < 0, true, fail2);
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
