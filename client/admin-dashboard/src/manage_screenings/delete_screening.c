#include <buracchi/common/utilities/try.h>
#include <cms/cms.h>
#include <cms/screenings_scheduling.h>
#include <cliutils/io.h>

#include "../core.h"

extern int select_screening(cms_t cms, struct cms_screening* screening);

extern int delete_screening(cms_t cms) {
	struct cms_delete_screening_request request = { 0 };
	struct cms_delete_screening_response* response;
	struct cms_screening screening;
	switch (select_screening(cms, &screening)) {
	case 1:
		goto fail;
	case 2:
		return 0;
	}
	io_clear_screen();
	puts(title);
	printf("Proiezione: %s - Sala %d il %s alle %s\n\n", screening.cinema_address, screening.hall_number, screening.date, screening.start_time);
	if (multi_choice("La proiezione selezionata verra' rimossa, procedere?", ((char[2]){ 'S', 'N' })) == 'N') {
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
	return 1;
}
