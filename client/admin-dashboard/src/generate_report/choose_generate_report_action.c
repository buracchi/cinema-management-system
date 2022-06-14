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
				break;
			case SHOW_CINEMA_WITHOUT_ENOUGH_USHERS:
				break;
			case SHOW_MONTHLY_BOOKING_STATE:
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
