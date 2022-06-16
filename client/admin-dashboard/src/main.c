#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

#include <buracchi/common/utilities/strto.h>
#include <buracchi/common/utilities/try.h>
#include <cms/cms.h>
#include <cliutils/io.h>
#include <cliutils/dotenv.h>

#include "generate_report/generate_report.h"
#include "manage_cinema/manage_cinema.h"
#include "manage_employees/manage_employees.h"
#include "manage_screenings/manage_screenings.h"
#include "manage_shifts/manage_shifts.h"
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
	try(cmn_strto_uint16((uint16_t*)&(credentials.port), getenv("PORT"), 10), 1, fail);
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
			try(choose_manage_cinema_action(cms), 1, fail2);
			break;
		case MANAGE_SCREENINGS:
			try(choose_manage_screenings_action(cms), 1, fail2);
			break;
		case MANAGE_SHIFTS:
			try(choose_manage_shifts_action(cms), 1, fail2);
			break;
		case MANAGE_EMPLOYEES:
			try(choose_manage_employees_action(cms), 1, fail2);
			break;
		case GENERATE_REPORT:
			try(choose_generate_report_action(cms), 1, fail2);
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
