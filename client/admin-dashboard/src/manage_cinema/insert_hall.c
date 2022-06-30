#include <buracchi/common/utilities/strto.h>
#include <buracchi/common/utilities/try.h>
#include <cms/cms.h>
#include <cms/cinema_management.h>
#include <cliutils/io.h>

#include "../core.h"

extern int select_cinema(cms_t cms, struct cms_cinema* cinema);

extern int insert_hall(cms_t cms) {
	struct cms_hall_details hall_details = {0 };
	struct cms_add_hall_response* response = NULL;
	struct cms_cinema cinema;
	char hall_number[INT32DSTR_LEN];
	char rows[INT32DSTR_LEN];
	char rows_seats[INT32DSTR_LEN];
	switch (select_cinema(cms, &cinema)) {
	case 1:
		goto fail;
	case 2:
		return 0;
	}
	while (true) {
		io_clear_screen();
		puts(title);
		printf("Cinema: %s\n\n", cinema.address);
		get_input("Numero sala: ", hall_number, false);
		get_input("Numero file: ", rows, false);
		get_input("Numero di posti per fila: ", rows_seats, false);
		puts("");
		if (multi_choice("Procedere?", ((char[]){ 'S', 'N' })) == 'N') {
			return 0;
		}
		if (cmn_strto_int32(&hall_details.hall_number, hall_number, 10)) {
			continue;
		}
		if (cmn_strto_int32(&hall_details.rows, rows, 10)) {
			continue;
		}
		if (cmn_strto_int32(&hall_details.rows_seats, rows_seats, 10)) {
			continue;
		}
		break;
	}
	puts("");
	hall_details.cinema_id = cinema.id;
	try(cms_add_hall(cms, &hall_details, &response), 1, fail);
	if (response->error_message) {
		printf("%s\n", response->error_message);
	}
	else {
		puts("Sala aggiunta con successo");
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
