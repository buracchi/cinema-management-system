#include <buracchi/common/utilities/try.h>
#include <cms/cms.h>
#include <cms/report.h>
#include <cliutils/io.h>
#include <fort.h>

#include "../core.h"

static char* get_reservations_table(struct cms_reservations_status* reservations_status, uint64_t num_elements);

extern int show_monthly_reservations_state(cms_t cms) {
	struct cms_response response;
	struct cms_reservations_status* reservations_status;
	io_clear_screen();
	puts(title);
	response = cms_get_reservations_status(cms, &reservations_status);
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
	char* booking_table;
	try(booking_table = get_reservations_table(reservations_status, response.num_elements), NULL, fail);
	puts(booking_table);
	free(booking_table);
	cms_destroy_response(&response);
	press_anykey();
	return 0;
fail:
	return 1;
}

static char* get_reservations_table(struct cms_reservations_status* reservations_status, uint64_t num_elements) {
	char* result;
	ft_table_t* table;
	const char* str_table;
	try(table = ft_create_table(), NULL, fail);
	try(ft_set_cell_prop(table, 0, FT_ANY_COLUMN, FT_CPROP_ROW_TYPE, FT_ROW_HEADER) < 0, true, fail2);
	try(ft_write_ln(table, "CINEMA", "INDIRIZZO", "SALA", "TOTALE", "CONFERMATE", "ANNULLATE", "VALIDATE", "SCADUTE") < 0, true, fail2);
	for (uint64_t i = 0; i < num_elements; i++) {
		char cinema[INT32DSTR_LEN] = { 0 };
		char hall[INT32DSTR_LEN] = { 0 };
		char* address = reservations_status[i].address;
		char* total = reservations_status[i].total;
		char* confirmed = reservations_status[i].confirmed;
		char* cancelled = reservations_status[i].cancelled;
		char* validated = reservations_status[i].validated;
		char* expired = reservations_status[i].expired;
		try(snprintf(cinema, INT32DSTR_LEN, "%d", reservations_status[i].cinema) < 0, true, fail2);
		try(snprintf(hall, INT32DSTR_LEN, "%d", reservations_status[i].hall) < 0, true, fail2);
		try(ft_write_ln(table, cinema, address, hall, total, confirmed, cancelled, validated, expired) < 0, true, fail2);
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
