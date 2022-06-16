#include <buracchi/common/utilities/try.h>
#include <cms/cms.h>
#include <cms/screenings_scheduling.h.>
#include <cms/cinema_management.h>
#include <cliutils/io.h>

#include "../core.h"

extern int select_cinema(cms_t cms, struct cms_cinema* cinema);
extern int select_hall(cms_t cms, int32_t cinema_id, struct cms_hall* hall);
extern int select_movie(cms_t cms, struct cms_movie* movie);

extern int insert_screening(cms_t cms) {
	struct cms_add_screening_request request = { 0 };
	struct cms_add_screening_response* response = NULL;
	struct cms_movie movie;
	struct cms_cinema cinema;
	struct cms_hall hall;
	switch (select_movie(cms, &movie)) {
		case 1:
			goto fail;
		case 2:
			return 0;
	}
	switch (select_cinema(cms, &cinema)) {
		case 1:
			goto fail;
		case 2:
			return 0;
	}
	switch (select_hall(cms, cinema.id, &hall)) {
		case 1:
			goto fail;
		case 2:
			return 0;
	}
	io_clear_screen();
	puts(title);
	get_input_len("Data [YYYY-MM-DD]: ", sizeof(request.date), (char*)request.date, false);
	get_input_len("Ora inizio [hh:mm:ss]: ", sizeof(request.start_time), (char*)request.start_time, false);
	get_input_len("Prezzo: ", sizeof(request.price), (char*)request.price, false);
	if (multi_choice("Procedere?", ((char[2]){ 'S', 'N' })) == 'N') {
		return 0;
	}
	request.film_id = movie.film_id;
	request.cinema_id = cinema.id;
	request.hall_number = hall.id;
	try(cms_add_screening(cms, &request, &response), 1, fail);
	if (response->error_message) {
		printf("%s", response->error_message);
	}
	else {
		puts("\nProiezione aggiunta con successo");
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
