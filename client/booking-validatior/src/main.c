#include <stdlib.h>
#include <stdio.h>

// port argparser to windows and include it here
#include "buracchi/libcommon/utilities/try.h"
#include "cinema-management-system.h"

int main(int argc, char** argv) {
	if (argc != 2) {
		fprintf(stderr, "stub of usage message");
		goto fail;
	}
	cinema_management_system_t cms;
	try(cms = cinema_management_system_init(), NULL, fail);
	return EXIT_SUCCESS;
fail:
	return EXIT_FAILURE;
}
