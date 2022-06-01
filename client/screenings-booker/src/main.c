#include <stdlib.h>
#include <stdio.h>

#include <buracchi/common/utilities/try.h>
#include <cms.h>

#include "utilities/io.h"

#define RUN_FROM_IDE

#define INT32DSTR_LEN 17

enum actions {
	MAKE_BOOKING,
	CANCEL_BOOKING,
	QUIT,
	END_OF_ACTIONS
};

static const char* title = "*********************************\n\
						  \r*       SCREENINGS BOOKER       *\n\
						  \r*********************************\n";

static int get_action(void);

int main(int argc, char** argv) {
#ifdef RUN_FROM_IDE
	putenv("HOST=localhost");
	putenv("DB=cinemadb");
	putenv("PORT=3306");
	putenv("CUSTOMER_USERNAME=cliente");
	putenv("CUSTOMER_PASSWORD=pippo");
#endif
	bool end = false;
	cms_t cms;
	char* username = getenv("CUSTOMER_USERNAME");
	char* password = getenv("CUSTOMER_PASSWORD");
	try(cms = cms_init(username, password), NULL, fail);
	while (!end) {
		int action = get_action();
		switch (action) {
		case MAKE_BOOKING:
			try(make_booking(cms), 1, fail);
			break;
		case CANCEL_BOOKING:
			try(cancel_booking(cms), 1, fail);
			break;
		case QUIT:
			end = true;
			break;
		default:
			fprintf(stderr, "Error: unknown action\n");
			break;
		}
		press_anykey();
	}
	cms_destroy(cms);
	return EXIT_SUCCESS;
fail:
	fprintf(stderr, cms_get_error_message(cms));
	cms_destroy(cms);
	return EXIT_FAILURE;
}

static int get_action(void) {
	char options[3] = { '1', '2', '3' };
	char op;
	clear_screen();
	puts(title);
	puts("Choose an action\n");
	puts("1) Make booking");
	puts("2) Cancel booking");
	puts("3) Quit");
	puts("\n");
	op = multi_choice("Select an option", options, 3);
	return op - '1';
}
