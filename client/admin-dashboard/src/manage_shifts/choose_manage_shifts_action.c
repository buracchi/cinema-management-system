#include "manage_shifts.h"

#include <stdio.h>
#include <stdbool.h>

#include <buracchi/common/utilities/try.h>
#include <cliutils/io.h>

#include "../core.h"

enum actions {
	SHOW_SHIFTS = '1',
	ADD_SHIFTS = '2',
	REMOVE_SHIFTS = '3',
	BACK_TO_MENU = 'Q'
};

extern int show_shift(cms_t cms);
extern int insert_shift(cms_t cms);
extern int delete_shift(cms_t cms);

extern int choose_manage_shifts_action(cms_t cms) {
	bool end = false;
	while (!end) {
		char action;
		io_clear_screen();
		puts(title);
		puts("Scegliere un'azione\n");
		puts("1) Visualizzare i turni in programma");
		puts("2) Aggiungere nuovi turni");
		puts("3) Eliminare turni esistenti in programma");
		puts("Q) Tornare al menu' principale");
		puts("\n");
		action = multi_choice("Selezionare un opzione", ((char[]){ '1', '2', '3', 'Q'}));
		switch (action) {
		case SHOW_SHIFTS:
			try(show_shift(cms), 1, fail);
			break;
		case ADD_SHIFTS:
			try(insert_shift(cms), 1, fail);
			break;
		case REMOVE_SHIFTS:
			try(delete_shift(cms), 1, fail);
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
