#include <buracchi/common/utilities/try.h>
#include <cms/cms.h>
#include <cms/projections_scheduling.h>
#include <cms/cinema_management.h>
#include <cliutils/io.h>

#include "../core.h"

extern int select_cinema(cms_t cms, struct cms_cinema* cinema);
extern int select_hall(cms_t cms, int32_t cinema_id, struct cms_hall* selected_hall);
extern int select_movie(cms_t cms, struct cms_movie* movie);

extern int insert_projection(cms_t cms) {
	struct cms_add_projection_details add_projection_details = { 0 };
	struct cms_response response;
	struct cms_cinema cinema = { 0 };
	struct cms_hall hall = { 0 };
	struct cms_movie movie = { 0 };
	while (true) {
		bool back = false;
		switch (select_cinema(cms, &cinema)) {
		case 1:
			return 1;
		case 2:
			return 0;
		}
		while (true) {
			if (back) {
				break;
			}
			switch (select_hall(cms, cinema.id, &hall)) {
			case 1:
				return 1;
			case 2:
				back = true;
				continue;
			}
			switch (select_movie(cms, &movie)) {
			case 1:
				return 1;
			case 2:
				continue;
			}
			break;
		}
		if (!back) {
			break;
		}
	}
	io_clear_screen();
	puts(title);
	printf("Cinema: %s\n", cinema.address);
	printf("Sala: %d\n", hall.id);
	printf("Film: %s\n", movie.name);
	get_input("Data [YYYY-MM-DD]: ", add_projection_details.date, false);
	get_input("Ora inizio [hh:mm:ss]: ", add_projection_details.start_time, false);
	get_input("Prezzo: ", add_projection_details.price, false);
	puts("");
	if (multi_choice("Procedere?", ((char[]){ 'S', 'N' })) == 'N') {
		return 0;
	}
	puts("");
	add_projection_details.film_id = movie.film_id;
	add_projection_details.cinema_id = cinema.id;
	add_projection_details.hall_number = hall.id;
	response = cms_add_projection(cms, &add_projection_details);
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
	puts("Proiezione aggiunta con successo");
	cms_destroy_response(&response);
	press_anykey();
	return 0;
}
