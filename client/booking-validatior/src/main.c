#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>

// port argparser to windows and include it here
#include "buracchi/libcommon/utilities/try.h"
#include "buracchi/libcommon/utilities/utilities.h"
#include "cinema-management-service.h"
#include "booking.h"

int main(int argc, char** argv) {
	if (argc != 2) {
		fprintf(stderr, "stub of usage message");
		goto fail;
	}
	int32_t booking_code;
	try(strtoi(argv[1], &booking_code), 1, fail);
	cinema_management_service_t cms;
	try(cms = cinema_management_service_init(getenv("PROJECTIONIST_USERNAME"), getenv("PROJECTIONIST_PASSWORD")), NULL, fail);
	try(validate_booking(cms, { booking_code }, NULL), !0, fail);
	printf("Validated\n");
	try(cinema_management_service_destroy(cms), !0, fail);
	return EXIT_SUCCESS;
fail:
	return EXIT_FAILURE;
}
