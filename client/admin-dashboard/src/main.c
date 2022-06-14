#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

#include <buracchi/common/utilities/try.h>
#include <cms/cms.h>
#include <cliutils/io.h>
#include <cliutils/dotenv.h>
#include <cliutils/strto.h>

#include "core.h"

enum actions {
	MANAGE_CINEMA,
	MANAGE_SCREENINGS,
	MANAGE_SHIFTS,
	MANAGE_EMPLOYEES,
	GENERATE_REPORT,
	QUIT
};

int main(void) {
    cms_t cms;
    bool end = false;
    try(env_load(".", false), -1, fail);
    struct cms_credentials credentials = {
		.username = getenv("ADMIN_USERNAME"),
		.password = getenv("ADMIN_PASSWORD"),
		.host = getenv("HOST"),
		.db = getenv("DB")
    };
    try(strtouint16((uint16_t *) &(credentials.port), getenv("PORT"), 10) == STRTO_SUCCESS, false, fail);
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
