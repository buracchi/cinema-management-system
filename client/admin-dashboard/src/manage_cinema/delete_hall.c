#include <buracchi/common/utilities/try.h>
#include <cms/cms.h>
#include <cms/cinema_management.h>
#include <cliutils/io.h>

#include "../core.h"

extern int select_cinema(cms_t cms, struct cms_cinema* cinema);
extern int select_hall(cms_t cms, int32_t cinema_id, int32_t* hall_number);

extern int delete_hall(cms_t cms) {
	struct cms_delete_hall_request request = { 0 };
	struct cms_delete_hall_response* response;
	struct cms_cinema cinema;
	switch (select_cinema(cms, &cinema)) {
	case 1:
		goto fail;
	case 2:
		return 0;
	}
	switch (select_hall(cms, cinema.id, &(request.hall_number))) {
	case 1:
		goto fail;
	case 2:
		return 0;
	}
	io_clear_screen();
	puts(title);
	printf("Cinema: %s\n", cinema.address);
	printf("Sala: %d\n\n", request.hall_number);
	if (multi_choice("La sala selezionata verra' rimossa, procedere?", ((char[2]){ 'S', 'N' })) == 'N') {
		return 0;
	}
	request.cinema_id = cinema.id;
	try(cms_delete_hall(cms, request, &response), 1, fail);
	if (response->error_message) {
		printf("%s", response->error_message);
	}
	else {
		puts("\nSala rimossa con successo");
	}
	cms_destroy_response((struct cms_response*)response);
	press_anykey();
	return 0;
fail:
	return 1;
}
