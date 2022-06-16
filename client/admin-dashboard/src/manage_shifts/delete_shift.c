#include <buracchi/common/utilities/try.h>
#include <cms/cms.h>
#include <cms/shift_scheduling.h>
#include <cliutils/io.h>

#include "../core.h"

extern int select_shift(cms_t cms, struct cms_shift* shift);

extern int delete_shift(cms_t cms) {
	struct cms_delete_shift_request request = { 0 };
	struct cms_delete_shift_response* response;
	struct cms_shift shift;
	switch (select_shift(cms, &shift)) {
	case 1:
		goto fail;
	case 2:
		return 0;
	}
	io_clear_screen();
	puts(title);
	if (multi_choice("Il turno selezionato verra' rimosso, procedere?", ((char[2]){ 'S', 'N' })) == 'N') {
		return 0;
	}
	request.employee_id = shift.employee_id;
	memcpy(request.day, shift.day, sizeof(request.day));
	memcpy(request.start_time, shift.start_time, sizeof(request.start_time));
	try(cms_delete_shift(cms, request, &response), 1, fail);
	if (response->error_message) {
		printf("%s", response->error_message);
	}
	else {
		puts("\nTurno rimosso con successo");
	}
	cms_destroy_response((struct cms_response*)response);
	press_anykey();
	return 0;
fail:
	return 1;
}
