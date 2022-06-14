#include <buracchi/common/utilities/try.h>
#include <cms/cms.h>
#include <cms/cinema_management.h>
#include <cliutils/io.h>
#include <cliutils/strto.h>

#include "../core.h"

extern int select_cinema(cms_t cms, struct cms_cinema* cinema);

extern int insert_hall(cms_t cms) {
	struct cms_add_hall_request request = { 0 };
	struct cms_add_hall_response* response;
	struct cms_cinema cinema;
	char hall_number[INT32DSTR_LEN];
	char rows[INT32DSTR_LEN];
	char rows_seats[INT32DSTR_LEN];
	switch (select_cinema(cms, &cinema)) {
	case 1:
		goto fail;
	case 2:
		return 0;
	};
	io_clear_screen();
	puts(title);
	printf("Cinema: %s\n\n", cinema.address);
	get_input("Numero sala: ", hall_number, false);
	get_input("Numero file: ", rows, false);
	get_input("Numero di posti per fila: ", rows_seats, false);
	if (multi_choice("Procedere?", ((char[2]){ 'S', 'N' })) == 'N') {
		return 0;
	}
	request.cinema_id = cinema.id;
	try(strtoint32(&request.hall_number, hall_number, 10) == STRTO_SUCCESS, false, fail);
	try(strtoint32(&request.rows, rows, 10) == STRTO_SUCCESS, false, fail);
	try(strtoint32(&request.rows_seats, rows_seats, 10) == STRTO_SUCCESS, false, fail);
	try(cms_add_hall(cms, request, &response), 1, fail);
	if (response->error_message) {
		printf("%s", response->error_message);
	}
	else {
		puts("\nSala aggiunta con successo");
	}
	cms_destroy_response((struct cms_response*)response);
	press_anykey();
	return 0;
fail:
	return 1;
}
