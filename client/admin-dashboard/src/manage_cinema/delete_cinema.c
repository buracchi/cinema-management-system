#include <buracchi/common/utilities/try.h>
#include <cms/cms.h>
#include <cms/cinema_management.h>
#include <cliutils/io.h>

#include "../core.h"

extern int select_cinema(cms_t cms, struct cms_cinema* cinema);

extern int delete_cinema(cms_t cms) {
	struct cms_delete_cinema_request request = { 0 };
	struct cms_delete_cinema_response* response;
	struct cms_cinema cinema;
	switch (select_cinema(cms, &cinema)) {
	case 1:
		goto fail;
	case 2:
		return 0;
	}
	io_clear_screen();
	puts(title);
	printf("Cinema: %s\n\n", cinema.address);
	if (multi_choice("Il cinema selezionato verra' rimosso, procedere?", ((char[2]){ 'S', 'N' })) == 'N') {
		return 0;
	}
	request.id = cinema.id;
	try(cms_delete_cinema(cms, request, &response), 1, fail);
	if (response->error_message) {
		printf("%s", response->error_message);
	}
	else {
		puts("\nCinema rimosso con successo");
	}
	cms_destroy_response((struct cms_response*)response);
	press_anykey();
	return 0;
fail:
	return 1;
}
