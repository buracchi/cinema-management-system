#include <buracchi/common/utilities/try.h>
#include <cms/cms.h>
#include <cms/cinema_management.h>
#include <cliutils/io.h>

#include "../core.h"

extern int select_cinema(cms_t cms, struct cms_cinema* cinema);

extern int delete_cinema(cms_t cms) {
	struct cms_response response;
	struct cms_cinema cinema;
	switch (select_cinema(cms, &cinema)) {
	case 1:
		return 1;
	case 2:
		return 0;
	}
	io_clear_screen();
	puts(title);
	printf("Cinema: %s\n\n", cinema.address);
	if (multi_choice("Il cinema selezionato verra' rimosso, procedere?", ((char[]){ 'S', 'N' })) == 'N') {
		return 0;
	}
	puts("");
	response = cms_delete_cinema(cms, cinema.id);
	if (response.fatal_error) {
		fprintf(stderr, "%s\n", response.error_message ? response.error_message : cms_get_error_message(cms));
		cms_destroy_response(&response);
		return 1;
	}
	if (response.error_message) {
		printf("%s\n", response.error_message);
		cms_destroy_response(&response);
		press_anykey();
		return 0;
	}
	puts("Cinema rimosso con successo");
	cms_destroy_response(&response);
	press_anykey();
	return 0;
}
