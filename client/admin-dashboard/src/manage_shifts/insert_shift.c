#include <buracchi/common/utilities/try.h>
#include <cms/cms.h>
#include <cms/shift_scheduling.h>
#include <cms/employee_management.h>
#include <cms/cinema_management.h>
#include <cliutils/io.h>

#include "../core.h"

extern int select_cinema(cms_t cms, struct cms_cinema* cinema);
extern int select_employee(cms_t cms, struct cms_employee* employee);

extern int insert_shift(cms_t cms) {
	struct cms_add_shift_request request = { 0 };
	struct cms_add_shift_response* response = NULL;
	struct cms_cinema cinema;
	struct cms_employee employee;
	switch (select_employee(cms, &employee)) {
		case 1:
			goto fail;
		case 2:
			return 0;
	}
	switch (select_cinema(cms, &cinema)) {
		case 1:
			goto fail;
		case 2:
			return 0;
	}
	io_clear_screen();
	puts(title);
	get_input_len("Giorno <Lunedi' | Martedi' | Mercoledi' | Giovedi' | Venerdi' | Sabato | Domenica>: ", sizeof(request.day), (char*)request.day, false);
	get_input_len("Ora inizio [hh:mm:ss]: ", sizeof(request.start_time), (char*)request.start_time, false);
	get_input_len("Durata [hh:mm:ss]: ", sizeof(request.duration), (char*)request.duration, false);
	if (multi_choice("Procedere?", ((char[2]){ 'S', 'N' })) == 'N') {
		return 0;
	}
	request.employee_id = employee.id;
	request.cinema_id = cinema.id;
	try(cms_add_shift(cms, &request, &response), 1, fail);
	if (response->error_message) {
		printf("%s", response->error_message);
	}
	else {
		puts("\nProiezione aggiunta con successo");
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
