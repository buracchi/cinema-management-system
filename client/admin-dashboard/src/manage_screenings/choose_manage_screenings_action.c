#include "manage_screenings.h"

#include <stdio.h>
#include <stdbool.h>

#include <buracchi/common/utilities/try.h>
#include <cliutils/io.h>

#include "../core.h"

enum actions {
	SHOW_SCREENINGS,
	ADD_SCREENINGS,
	REMOVE_SCREENINGS,
	ASSIGN_PROJECTIONIST,
	BACK_TO_MENU
};

extern int choose_manage_screenings_action(cms_t cms) {
	bool end = false;
	while (!end) {
		int action;
		io_clear_screen();
		puts(title);
		puts("Scegliere un'azione\n");
		puts("1) Visualizzare le proiezioni in programma");
		puts("2) Aggiungere nuove proiezioni");
		puts("3) Eliminare proiezioni esistenti in programma");
		puts("4) Assegnare ad una proiezione in programma un proiezionista");
		puts("5) Tornare al menu' principale");
		puts("\n");
		action = multi_choice("Selezionare un opzione", ((char[5]){ '1', '2', '3', '4', '5' })) - '1';
		switch (action) {
		case SHOW_SCREENINGS:
			break;
		case ADD_SCREENINGS:
			break;
		case REMOVE_SCREENINGS:
			break;
		case ASSIGN_PROJECTIONIST:
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
