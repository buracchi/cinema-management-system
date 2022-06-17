#include <buracchi/common/utilities/try.h>
#include <cms/cms.h>
#include <cms/screenings_scheduling.h>
#include <cliutils/io.h>

#include "../core.h"

extern int select_screening(cms_t cms, struct cms_screening* screening);

extern int delete_screening(cms_t cms) {
	struct cms_delete_screening_request request = { 0 };
	struct cms_delete_screening_response* response = NULL;
	struct cms_screening screening;
	switch (select_screening(cms, &screening)) {
	case 1:
		goto fail;
	case 2:
		return 0;
	}
	io_clear_screen();
	puts(title);
	printf("Cinema: %s\n", screening.cinema_address);
	printf("Sala: %d\n", screening.hall_number);
	printf("Film: %s\n", screening.film_name);
	printf("Data: %s\n", screening.date);
	printf("Ora: %s\n\n", screening.start_time);
	if (multi_choice("La proiezione selezionata verra' rimossa, procedere?", ((char[]){ 'S', 'N' })) == 'N') {
		return 0;
	}
	request.cinema_id = screening.cinema_id;
	request.hall_number = screening.hall_number;
	memcpy(request.date, screening.date, sizeof(request.date));
	memcpy(request.start_time, screening.start_time, sizeof(request.start_time));
	try(cms_delete_screening(cms, &request, &response), 1, fail);
	if (response->error_message) {
		printf("%s", response->error_message);
	}
	else {
		puts("\nProiezione rimossa con successo");
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
