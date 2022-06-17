#include "manage_cinema.h"

#include <stdio.h>
#include <stdbool.h>

#include <buracchi/common/utilities/try.h>
#include <cliutils/io.h>

#include "../core.h"

enum actions {
	SHOW_CINEMA = '1',
	ADD_CINEMA = '2',
	REMOVE_CINEMA = '3',
	SHOW_HALLS = '4',
	ADD_HALL = '5',
	REMOVE_HALL = '6',
	BACK_TO_MENU = 'Q'
};

extern int show_cinema(cms_t cms);
extern int insert_cinema(cms_t cms);
extern int delete_cinema(cms_t cms);
extern int show_halls(cms_t cms);
extern int insert_hall(cms_t cms);
extern int delete_hall(cms_t cms);

extern int choose_manage_cinema_action(cms_t cms) {
	bool end = false;
	while (!end) {
		char action;
		io_clear_screen();
		puts(title);
		puts("Scegliere un'azione\n");
		puts("1) Visualizzare le informazioni dei cinema");
		puts("2) Aggiungere un nuovo cinema");
		puts("3) Eliminare un cinema");
		puts("4) Visualizzare le informazioni delle sale di un cinema");
		puts("5) Aggiungere una sala ad un cinema");
		puts("6) Eliminare una sala di un cinema");
		puts("Q) Tornare al menu' principale");
		puts("\n");
		action = multi_choice("Selezionare un opzione", ((char[]){ '1', '2', '3', '4', '5', '6', 'Q' }));
		switch (action) {
		case SHOW_CINEMA:
			try(show_cinema(cms), 1, fail);
			break;
		case ADD_CINEMA:
			try(insert_cinema(cms), 1, fail);
			break;
		case REMOVE_CINEMA:
			try(delete_cinema(cms), 1, fail);
			break;
		case SHOW_HALLS:
			try(show_halls(cms), 1, fail);
			break;
		case ADD_HALL:
			try(insert_hall(cms), 1, fail);
			break;
		case REMOVE_HALL:
			try(delete_hall(cms), 1, fail);
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
