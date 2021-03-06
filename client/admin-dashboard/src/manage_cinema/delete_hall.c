#include <buracchi/common/utilities/try.h>
#include <cms/cms.h>
#include <cms/cinema_management.h>
#include <cliutils/io.h>

#include "../core.h"

extern int select_cinema(cms_t cms, struct cms_cinema* cinema);
extern int select_hall(cms_t cms, int32_t cinema_id, struct cms_hall* selected_hall);

extern int delete_hall(cms_t cms) {
	struct cms_response response;
	struct cms_cinema cinema;
	struct cms_hall hall;
	while (true) {
		switch (select_cinema(cms, &cinema)) {
		case 1:
			return 1;
		case 2:
			return 0;
		}
		switch (select_hall(cms, cinema.id, &hall)) {
		case 1:
			return 1;
		case 2:
			continue;
		}
		break;
	}
	io_clear_screen();
	puts(title);
	printf("Cinema: %s\n", cinema.address);
	printf("Sala: %d\n\n", hall.id);
	if (multi_choice("La sala selezionata verra' rimossa, procedere?", ((char[]){ 'S', 'N' })) == 'N') {
		return 0;
	}
	puts("");
	response = cms_delete_hall(cms, cinema.id, hall.id);
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
	puts("Sala rimossa con successo");
	cms_destroy_response(&response);
	press_anykey();
	return 0;
}
