#include <buracchi/common/utilities/try.h>
#include <cms/cms.h>
#include <cms/shift_scheduling.h>
#include <cliutils/io.h>

#include "../core.h"

extern int select_shift(cms_t cms, struct cms_shift* shift);

extern int delete_shift(cms_t cms) {
	struct cms_delete_shift_request request = { 0 };
	struct cms_delete_shift_response* response = NULL;
	struct cms_shift shift;
	switch (select_shift(cms, &shift)) {
	case 1:
		goto fail;
	case 2:
		return 0;
	}
	io_clear_screen();
	puts(title);
	printf("Cinema: %s\n", shift.cinema_address);
	printf("Dipendente: %d - %s %s\n", shift.employee_id, shift.employee_name, shift.employee_surname);
	printf("Giorno: %s\n", shift.day);
	printf("Ora inizio: %s\n", shift.start_time);
	printf("Durata: %s\n\n", shift.duration);
	if (multi_choice("Il turno selezionato verra' rimosso, procedere?", ((char[]){ 'S', 'N' })) == 'N') {
		return 0;
	}
	puts("");
	request.employee_id = shift.employee_id;
	memcpy(request.day, shift.day, sizeof(request.day));
	memcpy(request.start_time, shift.start_time, sizeof(request.start_time));
	try(cms_delete_shift(cms, &request, &response), 1, fail);
	if (response->error_message) {
		printf("%s\n", response->error_message);
	}
	else {
		puts("Turno rimosso con successo");
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
