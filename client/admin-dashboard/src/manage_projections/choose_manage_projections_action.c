#include "manage_projections.h"

#include <stdio.h>
#include <stdbool.h>

#include <buracchi/common/utilities/try.h>
#include <cliutils/io.h>

#include "../core.h"

enum actions {
	SHOW_PROJECTIONS = '1',
	ADD_PROJECTION = '2',
	REMOVE_PROJECTION = '3',
	ASSIGN_PROJECTIONIST = '4',
	BACK_TO_MENU = 'Q'
};

extern int show_projections(cms_t cms);
extern int insert_projection(cms_t cms);
extern int delete_projection(cms_t cms);
extern int assign_projectionist(cms_t cms);

extern int choose_manage_projections_action(cms_t cms) {
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
		action = multi_choice("Selezionare un opzione", ((char[]){ '1', '2', '3', '4', 'Q' }));
		switch (action) {
		case SHOW_PROJECTIONS:
			try(show_projections(cms), 1, fail);
			break;
		case ADD_PROJECTION:
			try(insert_projection(cms), 1, fail);
			break;
		case REMOVE_PROJECTION:
			try(delete_projection(cms), 1, fail);
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
