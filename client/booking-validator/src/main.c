#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>

#include <buracchi/common/argparser/argparser.h>
#include <buracchi/common/utilities/utilities.h>
#include <buracchi/common/utilities/strto.h>
#include <buracchi/common/utilities/try.h>

#include <cms/cms.h>
#include <cms/booking.h>

#include <cliutils/dotenv.h>

#define RUN_FROM_IDE

static int validate_booking(const char* booking_code);

#ifdef RUN_FROM_IDE
int main(void) {
	return validate_booking("1");
}
#else
int main(int argc, char** argv) {
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
	return validate_booking(booking_code);
fail:
	return EXIT_FAILURE;
}
#endif

static int validate_booking(const char* booking_code) {
	cms_t cms;
	try(env_load(".", false), -1, fail);
	struct cms_credentials credentials = {
			.username = getenv("USHER_USERNAME"),
			.password = getenv("USHER_PASSWORD"),
			.host = getenv("HOST"),
			.db = getenv("DB")
	};
	struct cms_validate_booking_request request;
	struct cms_validate_booking_response* response;
	try(cmn_strto_uint16((uint16_t *) &(credentials.port), getenv("PORT"), 10), 1, fail);
	try(cmn_strto_int32(&(request.booking_code), booking_code, 10), 1, fail);
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
