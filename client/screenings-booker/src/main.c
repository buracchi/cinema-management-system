#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

#include <buracchi/common/utilities/try.h>
#include <cms/cms.h>
#include <cliutils/io.h>
#include <cliutils/dotenv.h>
#include <cliutils/strto.h>

#include "make_booking/make_booking.h"
#include "cancel_booking/cancel_booking.h"
#include "core.h"

enum actions {
	MAKE_BOOKING,
	CANCEL_BOOKING,
	QUIT
};

int main(void) {
	cms_t cms;
	bool end = false;
    try(env_load(".", false), -1, fail);
	struct cms_credentials credentials = {
		.username = getenv("CUSTOMER_USERNAME"),
		.password = getenv("CUSTOMER_PASSWORD"),
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
		puts("1) Effettua prenotazione");
		puts("2) Cancella prenotazione");
		puts("3) Uscire");
		puts("\n");
		action = multi_choice("Selezionare un opzione", ((char[3]){ '1', '2', '3' })) - '1';
		switch (action) {
		case MAKE_BOOKING:
			struct booking_data booking_data = { 0 };
			try(choose_cinema(cms, &booking_data), 1, fail2);
			break;
		case CANCEL_BOOKING:
			try(cancel_booking(cms), 1, fail2);
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
