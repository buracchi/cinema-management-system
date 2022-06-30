#include <buracchi/common/utilities/try.h>
#include <cms/cms.h>
#include <cms/shift_scheduling.h>
#include <cliutils/io.h>

#include "../core.h"

extern int select_shift(cms_t cms, struct cms_shift_details* shift_details);

extern int delete_shift(cms_t cms) {
	struct cms_shift shift = {0 };
	struct cms_delete_shift_response* response = NULL;
	struct cms_shift_details shift_details;
	switch (select_shift(cms, &shift_details)) {
	case 1:
		goto fail;
	case 2:
		return 0;
	}
	io_clear_screen();
	puts(title);
	printf("Cinema: %s\n", shift_details.cinema_address);
	printf("Dipendente: %d - %s %s\n", shift_details.employee_id, shift_details.employee_name, shift_details.employee_surname);
	printf("Giorno: %s\n", shift_details.day);
	printf("Ora inizio: %s\n", shift_details.start_time);
	printf("Durata: %s\n\n", shift_details.duration);
	if (multi_choice("Il turno selezionato verra' rimosso, procedere?", ((char[]){ 'S', 'N' })) == 'N') {
		return 0;
	}
	puts("");
	shift.employee_id = shift_details.employee_id;
	strcpy(shift.day, shift_details.day);
	strcpy(shift.start_time, shift_details.start_time);
	try(cms_delete_shift(cms, &shift, &response), 1, fail);
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
