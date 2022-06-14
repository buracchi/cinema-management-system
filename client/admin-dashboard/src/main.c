#include <stdlib.h>
#include <stdio.h>

#include <buracchi/common/utilities/try.h>
#include <cms/cms.h>

#include "utilities/io.h"
#include "resources.h"

#define RUN_FROM_IDE

enum actions {
	MANAGE_CINEMA,
	MANAGE_SCREENINGS,
	MANAGE_SHIFTS,
	MANAGE_EMPLOYEES,
	GENERATE_REPORT,
	QUIT
};

int main(void) {
#ifdef RUN_FROM_IDE
	try(putenv("HOST=localhost") == 0, false, fail);
	try(putenv("DB=cinemadb") == 0, false, fail);
	try(putenv("PORT=3306") == 0, false, fail);
	try(putenv("ADMIN_USERNAME=amministratore") == 0, false, fail);
	try(putenv("ADMIN_PASSWORD=pippo") == 0, false, fail);
#endif
	bool end = false;
	cms_t cms;
	unsigned int port = atoi(getenv("PORT")); // TODO: sanitize
	struct cms_credentials credentials = {
		.username = getenv("ADMIN_USERNAME"),
		.password = getenv("ADMIN_PASSWORD"),
		.host = getenv("HOST"),
		.db = getenv("DB"),
		.port = port
	};
	try(cms = cms_init(&credentials), NULL, fail);
	while (!end) {
		int action;
		io_clear_screen();
		puts(title);
		puts("Scegliere un'azione\n");
		puts("1) Gestire i cinema");
		puts("2) Gestire la programmazione delle proiezioni");
		puts("3) Gestire la programmazione dei turni");
		puts("4) Gestire i dipendenti");
		puts("5) Generare report");
		puts("6) Uscire");
		puts("\n");
		action = multi_choice("Selezionare un opzione", ((char[6]){ '1', '2', '3', '4', '5', '6' })) - '1';
		switch (action) {
        case MANAGE_CINEMA:
            break;
        case MANAGE_SCREENINGS:
            break;
        case MANAGE_SHIFTS:
            break;
        case MANAGE_EMPLOYEES:
            break;
        case GENERATE_REPORT:
            break;
        case QUIT:
            end = true;
            break;
		default:
			fprintf(stderr, "Errore: l'azione scelta e' invalida\n");
			break;
		}
	}
	cms_destroy(cms);
	return EXIT_SUCCESS;
fail2:
	fprintf(stderr, "%s\n", cms_get_error_message(cms));
	cms_destroy(cms);
	return EXIT_FAILURE;
fail:
	fprintf(stderr, "Errore: impossibile connettersi al server, controllare le credenziali e riprovare in seguito.");
	return EXIT_FAILURE;
}
