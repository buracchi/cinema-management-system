#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

#include <buracchi/common/utilities/strto.h>
#include <buracchi/common/utilities/try.h>
#include <cms/cms.h>
#include <cliutils/io.h>
#include <cliutils/dotenv.h>

#include "make_booking/make_booking.h"
#include "cancel_booking/cancel_booking.h"
#include "core.h"

enum actions {
	MAKE_BOOKING = '1',
	CANCEL_BOOKING = '2',
	QUIT = 'Q'
};

int main(void) {
	cms_t cms;
	bool end = false;
	env_load(".", false);
	struct cms_credentials credentials = {
		.username = getenv("CUSTOMER_USERNAME"),
		.password = getenv("CUSTOMER_PASSWORD"),
		.host = getenv("HOST"),
		.db = getenv("DB")
	};
	try(cmn_strto_uint16((uint16_t*)&(credentials.port), getenv("PORT"), 10), 1, fail);
	try(cms = cms_init(&credentials), NULL, fail);
	while (!end) {
		char action;
		io_clear_screen();
		puts(title);
		puts("Scegliere un'azione\n");
		puts("1) Effettuare prenotazione");
		puts("2) Cancellare prenotazione");
		puts("Q) Uscire");
		puts("\n");
		action = multi_choice("Selezionare un opzione", ((char[]){ '1', '2', 'Q' }));
		switch (action) {
		case MAKE_BOOKING: {
			struct booking_data booking_data = { 0 };
			try(choose_cinema(cms, &booking_data), 1, fail2);
			break;
		}
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
	cms_destroy(cms);
	return EXIT_FAILURE;
fail:
	fprintf(stderr, "Errore: impossibile connettersi al server, controllare le credenziali e riprovare in seguito.\n");
	return EXIT_FAILURE;
}
