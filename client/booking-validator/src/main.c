#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>

// port argparser to windows and include it here
#include <buracchi/common/utilities/try.h>
#include <buracchi/common/utilities/utilities.h>

#include "cinema-management-service.h"
#include "booking.h"

void setup_env(void) {
	putenv("HOST=localhost");
	putenv("DB=cinemadb");
	putenv("PORT=3306");
	putenv("USHER_USERNAME=maschera");
	putenv("USHER_PASSWORD=pippo");
}

int main(int argc, char** argv) {
	if (argc != 2) {
		fprintf(stderr, "stub of usage message");
		goto fail;
	}
	setup_env();
	cinema_management_service_t cms;
	struct validate_booking_request request;
	struct validate_booking_response* response;
	char* username = getenv("USHER_USERNAME");
	char* password = getenv("USHER_PASSWORD");
	try(strtoi(argv[1], &(request.booking_code)), 1, fail);
	try(cms = cinema_management_service_init(username, password), NULL, fail);
	try(validate_booking(cms, request, NULL), !0, fail);
	printf("Validated\n");
	try(cinema_management_service_destroy(cms), !0, fail);
	return EXIT_SUCCESS;
fail:
	return EXIT_FAILURE;
}
