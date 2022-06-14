#include "generate_report.h"

#include <stdio.h>
#include <stdbool.h>

#include <buracchi/common/utilities/try.h>
#include <cliutils/io.h>

#include "../core.h"

enum actions {
	SHOW_SHIFTS_WITHOUT_PROJECTIONIST,
	SHOW_CINEMA_WITHOUT_ENOUGH_USHERS,
	SHOW_MONTHLY_BOOKING_STATE,
	BACK_TO_MENU
};

extern int show_cinema_without_enough_ushers(cms_t cms);
extern int show_screenings_without_projectionist(cms_t cms);
extern int show_monthly_booking_state(cms_t cms);

extern int choose_generate_report_action(cms_t cms) {
	bool end = false;
	while (!end) {
		int action;
		io_clear_screen();
		puts(title);
		puts("Scegliere un'azione\n");
		puts("1) Visualizzare le proiezioni sprovviste di proiezionisti");
		puts("2) Visualizzare i cinema sprovvisti di almeno due maschere");
		puts("3) Visualizzare lo stato delle prenotazioni di questo mese");
		puts("4) Tornare al menu' principale");
		puts("\n");
		action = multi_choice("Selezionare un opzione", ((char[4]){ '1', '2', '3', '4'})) - '1';
		switch (action) {
		case SHOW_SHIFTS_WITHOUT_PROJECTIONIST:
			try(show_screenings_without_projectionist(cms), 1, fail);
			break;
		case SHOW_CINEMA_WITHOUT_ENOUGH_USHERS:
			try(show_cinema_without_enough_ushers(cms), 1, fail);
			break;
		case SHOW_MONTHLY_BOOKING_STATE:
			try(show_monthly_booking_state(cms), 1, fail);
			break;
		case BACK_TO_MENU:
			end = true;
			break;
		default:
			fprintf(stderr, "Errore: l'azione scelta e' invalida\n");
			break;
		}
	}
	return 0;
fail:
	return 1;
}
