#include <buracchi/common/utilities/try.h>
#include <cms/cms.h>
#include <cms/cinema_management.h>
#include <cliutils/io.h>

#include "../core.h"

extern int insert_cinema(cms_t cms) {
	struct cms_cinema_details cinema_details = {0 };
	struct cms_add_cinema_response* response = NULL;
	io_clear_screen();
	puts(title);
	get_input("Indirizzo: ", cinema_details.address, false);
	get_input("Orario apertura [hh:mm:ss]: ", cinema_details.opening_time, false);
	get_input("Orario chiusura [hh:mm:ss]: ", cinema_details.closing_time, false);
	if (multi_choice("Procedere?", ((char[]){ 'S', 'N' })) == 'N') {
		return 0;
	}
	puts("");
	try(cms_add_cinema(cms, &cinema_details, &response), 1, fail);
	if (response->error_message) {
		printf("%s\n", response->error_message);
	}
	else {
		puts("Cinema aggiunto con successo");
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
