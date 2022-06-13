﻿#include <stdlib.h>
#include <stdio.h>

#include <buracchi/common/utilities/try.h>
#include <cms/cms.h>

#include "utilities/io.h"
#include "resources.h"

#include "make_booking.h"

#define RUN_FROM_IDE

enum actions {
	MAKE_BOOKING,
	CANCEL_BOOKING,
	QUIT,
	END_OF_ACTIONS
};

extern int cancel_booking(cms_t cms);

int main(void) {
#ifdef RUN_FROM_IDE
	try(putenv("HOST=localhost") == 0, false, fail);
	try(putenv("DB=cinemadb") == 0, false, fail);
	try(putenv("PORT=3306") == 0, false, fail);
	try(putenv("CUSTOMER_USERNAME=cliente") == 0, false, fail);
	try(putenv("CUSTOMER_PASSWORD=pippo") == 0, false, fail);
#endif
	bool end = false;
	cms_t cms;
	unsigned int port = atoi(getenv("PORT")); // TODO: sanitize
	struct cms_credentials credentials = {
		.username = getenv("CUSTOMER_USERNAME"),
		.password = getenv("CUSTOMER_PASSWORD"),
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
