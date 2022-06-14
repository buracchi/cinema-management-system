#include "manage_cinema.h"

#include <stdio.h>
#include <stdbool.h>

#include <buracchi/common/utilities/try.h>
#include <cliutils/io.h>

#include "../core.h"

enum actions {
	SHOW_CINEMA,
	ADD_CINEMA,
	REMOVE_CINEMA,
	SHOW_HALLS,
	ADD_HALL,
	REMOVE_HALL,
	BACK_TO_MENU
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
		int action;
		io_clear_screen();
		puts(title);
		puts("Scegliere un'azione\n");
		puts("1) Visualizzare le informazioni dei cinema");
		puts("2) Aggiungere un nuovo cinema");
		puts("3) Eliminare un cinema");
		puts("4) Visualizzare le informazioni delle sale di un cinema");
		puts("5) Aggiungere una sala ad un cinema");
		puts("6) Eliminare una sala di un cinema");
		puts("7) Tornare al menu' principale");
		puts("\n");
		action = multi_choice("Selezionare un opzione", ((char[7]){ '1', '2', '3', '4', '5', '6', '7' })) - '1';
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
