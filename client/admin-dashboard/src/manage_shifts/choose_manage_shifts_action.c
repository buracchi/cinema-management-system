#include "manage_shifts.h"

#include <stdio.h>
#include <stdbool.h>

#include <buracchi/common/utilities/try.h>
#include <cliutils/io.h>

#include "../core.h"

enum actions {
	SHOW_SHIFTS,
	ADD_SHIFTS,
	REMOVE_SHIFTS,
	BACK_TO_MENU
};

extern int choose_manage_shifts_action(cms_t cms) {
	bool end = false;
	while (!end) {
		int action;
		io_clear_screen();
		puts(title);
		puts("Scegliere un'azione\n");
		puts("1) Visualizzare i turni in programma");
		puts("2) Aggiungere nuovi turni");
		puts("3) Eliminare turni esistenti in programma");
		puts("4) Tornare al menu' principale");
		puts("\n");
		action = multi_choice("Selezionare un opzione", ((char[4]){ '1', '2', '3', '4'})) - '1';
		switch (action) {
			case SHOW_SHIFTS:
				break;
			case ADD_SHIFTS:
				break;
			case REMOVE_SHIFTS:
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
