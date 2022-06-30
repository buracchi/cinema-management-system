#include <buracchi/common/utilities/try.h>
#include <cms/cms.h>
#include <cms/projections_scheduling.h>
#include <cliutils/io.h>

#include "../core.h"

extern int select_projection(cms_t cms, struct cms_projection *projection);

extern int delete_projection(cms_t cms) {
	struct cms_delete_projection_request request = {0 };
	struct cms_delete_projection_response* response = NULL;
	struct cms_projection projection;
	switch (select_projection(cms, &projection)) {
	case 1:
		goto fail;
	case 2:
		return 0;
	}
	io_clear_screen();
	puts(title);
	printf("Cinema: %s\n", projection.cinema_address);
	printf("Sala: %d\n", projection.hall_number);
	printf("Film: %s\n", projection.film_name);
	printf("Data: %s\n", projection.date);
	printf("Ora: %s\n\n", projection.start_time);
	if (multi_choice("La proiezione selezionata verra' rimossa, procedere?", ((char[]){ 'S', 'N' })) == 'N') {
		return 0;
	}
	puts("");
	request.cinema_id = projection.cinema_id;
	request.hall_number = projection.hall_number;
	memcpy(request.date, projection.date, sizeof(request.date));
	memcpy(request.start_time, projection.start_time, sizeof(request.start_time));
	try(cms_delete_projection(cms, &request, &response), 1, fail);
	if (response->error_message) {
		printf("%s\n", response->error_message);
	}
	else {
		puts("Proiezione rimossa con successo");
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
