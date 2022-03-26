#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>

#include <buracchi/common/argparser/argparser.h>
#include <buracchi/common/utilities/utilities.h>
#include <buracchi/common/utilities/try.h>

#include "cinema-management-service.h"
#include "booking.h"

//#define RUN_FROM_IDE

static int validate(const char* booking_code);

int main(int argc, char** argv) {
#ifndef RUN_FROM_IDE
	char* booking_code;
	cmn_map_t option_map;
	cmn_argparser_t argparser;
	struct cmn_argparser_argument args[] = {
		{.name = "code", .help = "the booking code to be validated", .is_required = true }
	};
	try(argparser = cmn_argparser_init(argv[0], "Cinema booking validator."), NULL, fail);
	try(cmn_argparser_set_arguments(argparser, args, 1), 1, fail);
	option_map = cmn_argparser_parse(argparser, argc, argv);
	try(cmn_map_at(option_map, (void*)"code", (void**)&booking_code), 1, fail);
	try(cmn_argparser_destroy(argparser), 1, fail);
	return validate(booking_code);
fail:
	return EXIT_FAILURE;
#else
	return validate(NULL);
#endif
}

static int validate(const char* booking_code) {
#ifdef RUN_FROM_IDE
	putenv("HOST=localhost");
	putenv("DB=cinemadb");
	putenv("PORT=3306");
	putenv("USHER_USERNAME=maschera");
	putenv("USHER_PASSWORD=pippo");
	booking_code = "4";
#endif
	cinema_management_service_t cms;
	struct validate_booking_request request;
	struct validate_booking_response* response;
	char* username = getenv("USHER_USERNAME");
	char* password = getenv("USHER_PASSWORD");
	try(strtoi(booking_code, &(request.booking_code)), 1, fail);
	try(cms = cinema_management_service_init(username, password), NULL, fail);
	if (validate_booking(cms, request, &response)) {
		printf("%s\n", response->error_message);
		cinema_management_service_destroy(cms);
fail:
		return EXIT_FAILURE;
	}
	cinema_management_service_destroy(cms);
	return EXIT_SUCCESS;
}
