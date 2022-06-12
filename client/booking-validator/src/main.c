#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>

#include <buracchi/common/argparser/argparser.h>
#include <buracchi/common/utilities/utilities.h>
#include <buracchi/common/utilities/try.h>

#include <cms/cms.h>
#include <cms/booking.h>

//#define RUN_FROM_IDE

static int validate(const char* booking_code);

int main(int argc, char** argv) {
#ifdef RUN_FROM_IDE
	return validate(NULL);
#else
	char* booking_code;
	cmn_map_t option_map;
	cmn_argparser_t argparser;
	struct cmn_argparser_argument args[] = {
		{.name = "codice", .help = "il codice di prenotazione da validare", .is_required = true }
	};
	try(argparser = cmn_argparser_init(argv[0], "Validatore di prenotazioni."), NULL, fail);
	try(cmn_argparser_set_arguments(argparser, args, 1), 1, fail);
	option_map = cmn_argparser_parse(argparser, argc, (const char**) argv);
	try(cmn_map_at(option_map, (void*)"codice", (void**)&booking_code), 1, fail);
	try(cmn_argparser_destroy(argparser), 1, fail);
	return validate(booking_code);
fail:
	return EXIT_FAILURE;
#endif
}

static int validate(const char* booking_code) {
#ifdef RUN_FROM_IDE
	try(putenv("HOST=localhost") == 0, false, fail);
	try(putenv("DB=cinemadb") == 0, false, fail);
	try(putenv("PORT=3306") == 0, false, fail);
	try(putenv("USHER_USERNAME=maschera") == 0, false, fail);
	try(putenv("USHER_PASSWORD=pippo") == 0, false, fail);
	booking_code = "4";
#endif
	cms_t cms;
	unsigned int port = atoi(getenv("PORT"));
	struct cms_credentials credentials = {
		.host = getenv("HOST"),
		.db = getenv("DB"),
		.port = port,
		.username = getenv("USHER_USERNAME"),
		.password = getenv("USHER_PASSWORD")
	};
	struct cms_validate_booking_request request;
	struct cms_validate_booking_response* response;
	try(strtoi(booking_code, &(request.booking_code)), 1, fail);
	try(cms = cms_init(&credentials), NULL, fail);
	try(cms_validate_booking(cms, request, &response), 1, fail2);
	if (response->error_message) {
		fprintf(stderr, "%s\n", response->error_message);
		cms_destroy(cms);
		return EXIT_FAILURE;
	}
	printf("Prenotazione validata con successo.");
	cms_destroy(cms);
	return EXIT_SUCCESS;
fail2:
	fprintf(stderr, "%s\n", cms_get_error_message(cms));
	cms_destroy(cms);
fail:
	fprintf(stderr, "Impossibile connettersi al server, controllare le credenziali e riprovare in seguito.");
	return EXIT_FAILURE;
}
