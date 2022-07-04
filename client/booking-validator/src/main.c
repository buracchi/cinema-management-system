
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

//#define RUN_FROM_IDE

static int validate_booking(char(*booking_code)[CMS_BOOKING_CODE_LEN]);

#ifdef RUN_FROM_IDE
int main(void) {
	try(env_load(".", false), -1, fail);
	return validate_booking("123456");
}
#else
int main(int argc, char** argv) {
	char booking_code[CMS_BOOKING_CODE_LEN] = { 0 };
	char* input = NULL;
	cmn_map_t option_map;
	cmn_argparser_t argparser;
	struct cmn_argparser_argument args[] = {
		{.name = "codice", .help = "il codice di prenotazione da validare", .is_required = true }
	};
	try(argparser = cmn_argparser_init(argv[0], "Validatore di prenotazioni."), NULL, fail);
	try(cmn_argparser_set_arguments(argparser, args, 1), 1, fail);
	option_map = cmn_argparser_parse(argparser, argc, (const char**)argv);
	try(cmn_map_at(option_map, (void*)"codice", (void**)&input), 1, fail);
	try(cmn_argparser_destroy(argparser), 1, fail);
	if ((strlen(input) != sizeof booking_code - 1) || cmn_strto_uint32(&(uint32_t) { 0 }, input, 16)) {
		goto fail2;
	}
	strcpy(booking_code, input);
	try(env_load(".", false), -1, fail);
	return validate_booking(&booking_code);
fail2:
	fprintf(stderr, "Codice prenotazione non valido.");
fail:
	return EXIT_FAILURE;
}
#endif

static int validate_booking(char(*booking_code)[CMS_BOOKING_CODE_LEN]) {
	struct cms_response response;
	struct cms_credentials credentials = {
			.username = getenv("USHER_USERNAME"),
			.password = getenv("USHER_PASSWORD"),
			.host = getenv("HOST"),
			.db = getenv("DB")
	};
	cms_t cms;
	try(cmn_strto_uint16((uint16_t*)&(credentials.port), getenv("PORT"), 10), 1, fail);
	try(cms = cms_init(&credentials), NULL, fail);
	response = cms_validate_booking(cms, booking_code);
	if (response.fatal_error || response.error_message) {
		fprintf(stderr, "%s\n", response.error_message ? response.error_message : cms_get_error_message(cms));
		cms_destroy_response(&response);
		cms_destroy(cms);
		return EXIT_FAILURE;
	}
	printf("Prenotazione validata con successo.");
	cms_destroy_response(&response);
	cms_destroy(cms);
	return EXIT_SUCCESS;
fail:
	fprintf(stderr, "Impossibile connettersi al server, controllare le credenziali e riprovare in seguito.");
	return EXIT_FAILURE;
}
