#include <buracchi/common/utilities/try.h>
#include <cms/cms.h>
#include <cms/screenings_scheduling.h.>
#include <cliutils/io.h>

#include "../core.h"

extern int select_screening(cms_t cms, struct cms_screening* screening);
extern int select_projectionist(cms_t cms, struct cms_get_available_projectionists_request* request, struct cms_available_projectionist* projectionist);

extern int assign_projectionist(cms_t cms) {
	struct cms_assign_projectionist_request request = { 0 };
	struct cms_assign_projectionist_response* response = NULL;
	struct cms_screening screening;
	struct cms_available_projectionist projectionist;
	switch (select_screening(cms, &screening)) {
		case 1:
			goto fail;
		case 2:
			return 0;
	}
	struct cms_get_available_projectionists_request projectionists_request;
	projectionists_request.cinema_id = screening.cinema_id;
	projectionists_request.hall_number = screening.hall_number;
	memcpy(projectionists_request.date, screening.date, sizeof(projectionists_request.date));
	memcpy(projectionists_request.start_time, screening.start_time, sizeof(projectionists_request.start_time));
	switch (select_projectionist(cms, &projectionists_request, &projectionist)) {
		case 1:
			goto fail;
		case 2:
			return 0;
	}
	io_clear_screen();
	puts(title);
	printf("Proiezionista: %d - %s %s\n", projectionist.id, projectionist.name, projectionist.surname);
	printf("Cinema: %s\n", screening.cinema_address);
	printf("Sala: %d\n", screening.hall_number);
	printf("Film: %s\n", screening.film_name);
	printf("Data: %s\n", screening.date);
	printf("Ora: %s\n\n", screening.start_time);
	if (multi_choice("Procedere?", ((char[]){ 'S', 'N' })) == 'N') {
		return 0;
	}
	request.projectionist_id = projectionist.id;
	request.cinema_id = screening.cinema_id;
	request.hall_number = screening.hall_number;
	memcpy(request.date, screening.date, sizeof(request.date));
	memcpy(request.start_time, screening.start_time, sizeof(request.start_time));
	try(cms_assign_projectionist(cms, &request, &response), 1, fail);
	if (response->error_message) {
		printf("%s", response->error_message);
	}
	else {
		puts("\nProiezionista assegnato con successo");
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
