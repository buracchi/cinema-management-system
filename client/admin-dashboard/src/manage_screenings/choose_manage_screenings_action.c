#include "manage_screenings.h"

#include <stdio.h>
#include <stdbool.h>

#include <buracchi/common/utilities/try.h>
#include <cliutils/io.h>

#include "../core.h"

enum actions {
	SHOW_SCREENINGS = '1',
	ADD_SCREENINGS = '2',
	REMOVE_SCREENINGS = '3',
	ASSIGN_PROJECTIONIST = '4',
	BACK_TO_MENU = 'Q'
};

extern int show_screenings(cms_t cms);
extern int insert_screening(cms_t cms);
extern int delete_screening(cms_t cms);
extern int assign_projectionist(cms_t cms);

extern int choose_manage_screenings_action(cms_t cms) {
	bool end = false;
	while (!end) {
		char action;
		io_clear_screen();
		puts(title);
		puts("Scegliere un'azione\n");
		puts("1) Visualizzare le proiezioni in programma");
		puts("2) Aggiungere nuove proiezioni");
		puts("3) Eliminare proiezioni esistenti in programma");
		puts("4) Assegnare ad una proiezione in programma un proiezionista");
		puts("Q) Tornare al menu' principale");
		puts("\n");
		action = multi_choice("Selezionare un opzione", ((char[5]){ '1', '2', '3', '4', 'Q' }));
		switch (action) {
		case SHOW_SCREENINGS:
			try(show_screenings(cms), 1, fail);
			break;
		case ADD_SCREENINGS:
			try(insert_screening(cms), 1, fail);
			break;
		case REMOVE_SCREENINGS:
			try(delete_screening(cms), 1, fail);
			break;
		case ASSIGN_PROJECTIONIST:
			try(assign_projectionist(cms), 1, fail);
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
