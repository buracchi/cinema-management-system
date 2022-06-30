#include <buracchi/common/utilities/try.h>
#include <cms/cms.h>
#include <cms/shift_scheduling.h>
#include <cms/employee_management.h>
#include <cms/cinema_management.h>
#include <cliutils/io.h>

#include "../core.h"

extern int select_cinema(cms_t cms, struct cms_cinema* cinema);
extern int select_employee(cms_t cms, struct cms_employee* employee);
static char* get_day(char* day);

extern int insert_shift(cms_t cms) {
	struct cms_add_shift_details shift_details = {0 };
	struct cms_add_shift_response* response = NULL;
	struct cms_cinema cinema;
	struct cms_employee employee;
	while (true) {
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
				continue;
		}
		break;
	}
	io_clear_screen();
	puts(title);
	printf("Cinema: %s\n", cinema.address);
	printf("Dipendente: %d - %s %s\n", employee.id, employee.name, employee.surname);
	get_day((char*)shift_details.day);
	get_input("Ora inizio [hh:mm:ss]: ", shift_details.start_time, false);
	get_input("Durata [hh:mm:ss]: ", shift_details.duration, false);
	if (multi_choice("Procedere?", ((char[]){ 'S', 'N' })) == 'N') {
		return 0;
	}
	puts("");
	shift_details.employee_id = employee.id;
	shift_details.cinema_id = cinema.id;
	try(cms_add_shift(cms, &shift_details, &response), 1, fail);
	if (response->error_message) {
		printf("%s\n", response->error_message);
	}
	else {
		puts("Proiezione aggiunta con successo");
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

static char* get_day(char* day) {
	const char* days[] = { "Lunedi'", "Martedi'", "Mercoledi'", "Giovedi'", "Venerdi'", "Sabato", "Domenica" };
	size_t selection = multi_choice(
			"Giorno <1=Lunedi' | 2=Martedi' | 3=Mercoledi' | 4=Giovedi' | 5=Venerdi' | 6=Sabato | 7=Domenica> ",
			((char[]){ '1', '2', '3', '4', '5', '6', '7' })) - '1';
	strcpy(day, days[selection]);
	return day;
}
