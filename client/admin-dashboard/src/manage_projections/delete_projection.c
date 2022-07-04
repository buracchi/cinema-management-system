#include <buracchi/common/utilities/try.h>
#include <cms/cms.h>
#include <cms/projections_scheduling.h>
#include <cliutils/io.h>

#include "../core.h"

extern int select_projection(cms_t cms, struct cms_projection_details* selected_projection);

extern int delete_projection(cms_t cms) {
	struct cms_response response;
	struct cms_projection projection = { 0 };
	struct cms_projection_details projection_details;
	switch (select_projection(cms, &projection_details)) {
	case 1:
		return 1;
	case 2:
		return 0;
	}
	io_clear_screen();
	puts(title);
	printf("Cinema: %s\n", projection_details.cinema_address);
	printf("Sala: %d\n", projection_details.hall_number);
	printf("Film: %s\n", projection_details.film_name);
	printf("Data: %s\n", projection_details.date);
	printf("Ora: %s\n\n", projection_details.start_time);
	if (multi_choice("La proiezione selezionata verra' rimossa, procedere?", ((char[]){ 'S', 'N' })) == 'N') {
		return 0;
	}
	puts("");
	projection.cinema_id = projection_details.cinema_id;
	projection.hall_number = projection_details.hall_number;
	memcpy(projection.date, projection_details.date, sizeof(projection.date));
	memcpy(projection.start_time, projection_details.start_time, sizeof(projection.start_time));
	response = cms_delete_projection(cms, &projection);
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
	puts("Proiezione rimossa con successo");
	cms_destroy_response(&response);
	press_anykey();
	return 0;
}
