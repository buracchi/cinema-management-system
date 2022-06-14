#include "manage_employees.h"

#include <stdio.h>
#include <stdbool.h>

#include <buracchi/common/utilities/try.h>
#include <cliutils/io.h>

#include "../core.h"

enum actions {
	SHOW_EMPLOYEES,
	ADD_EMPLOYEE,
	REMOVE_EMPLOYEE,
	BACK_TO_MENU
};

extern int choose_manage_employees_action(cms_t cms) {
	bool end = false;
	while (!end) {
		int action;
		io_clear_screen();
		puts(title);
		puts("Scegliere un'azione\n");
		puts("1) Visualizzare i dipendenti");
		puts("2) Aggiungere un dipendente");
		puts("3) Eliminare un dipendente");
		puts("4) Tornare al menu' principale");
		puts("\n");
		action = multi_choice("Selezionare un opzione", ((char[4]){ '1', '2', '3', '4'})) - '1';
		switch (action) {
			case SHOW_EMPLOYEES:
				break;
			case ADD_EMPLOYEE:
				break;
			case REMOVE_EMPLOYEE:
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