#include <buracchi/common/utilities/try.h>
#include <cms/cms.h>
#include <cms/cinema_management.h>
#include <cliutils/io.h>

#include "../core.h"

extern int insert_cinema(cms_t cms) {
	struct cms_add_cinema_request request = { 0 };
	struct cms_add_cinema_response* response = NULL;
	io_clear_screen();
	puts(title);
	get_input_len("Indirizzo: ", sizeof(request.address), (char*)request.address, false);
	get_input_len("Orario apertura [hh:mm:ss]: ", sizeof(request.opening_time), (char*)request.opening_time, false);
	get_input_len("Orario chiusura [hh:mm:ss]: ", sizeof(request.closing_time), (char*)request.closing_time, false);
	if (multi_choice("Procedere?", ((char[2]){ 'S', 'N' })) == 'N') {
		return 0;
	}
	try(cms_add_cinema(cms, &request, &response), 1, fail);
	if (response->error_message) {
		printf("%s", response->error_message);
	}
	else {
		puts("\nCinema aggiunto con successo");
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
