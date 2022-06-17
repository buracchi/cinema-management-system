#include "manage_employees.h"

#include <stdio.h>
#include <stdbool.h>

#include <buracchi/common/utilities/try.h>
#include <cliutils/io.h>

#include "../core.h"

enum actions {
	SHOW_EMPLOYEES = '1',
	ADD_EMPLOYEE = '2',
	REMOVE_EMPLOYEE = '3',
	BACK_TO_MENU = 'Q'
};

extern int show_employees(cms_t cms);
extern int insert_employee(cms_t cms);
extern int delete_employee(cms_t cms);

extern int choose_manage_employees_action(cms_t cms) {
	bool end = false;
	while (!end) {
		char action;
		io_clear_screen();
		puts(title);
		puts("Scegliere un'azione\n");
		puts("1) Visualizzare i dipendenti");
		puts("2) Aggiungere un dipendente");
		puts("3) Eliminare un dipendente");
		puts("Q) Tornare al menu' principale");
		puts("\n");
		action = multi_choice("Selezionare un opzione", ((char[]){ '1', '2', '3', 'Q'}));
		switch (action) {
		case SHOW_EMPLOYEES:
			try(show_employees(cms), 1, fail);
			break;
		case ADD_EMPLOYEE:
			try(insert_employee(cms), 1, fail);
			break;
		case REMOVE_EMPLOYEE:
			try(delete_employee(cms), 1, fail);
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
