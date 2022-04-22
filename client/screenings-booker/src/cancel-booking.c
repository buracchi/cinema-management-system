#include "cancel-booking.h"

#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>

#include <buracchi/common/utilities/utilities.h>
#include <buracchi/common/utilities/try.h>
#include <fort.h>

#include "booking.h"
#include "utilities/io.h"

#define INT32DSTR_LEN 17

static const char* title = "*********************************\n\
						  \r*       SCREENINGS BOOKER       *\n\
						  \r*********************************\n";

extern int cancel_booking(cms_t cms) {
	struct cms_cancel_booking_request request = { 0 };
	struct cms_cancel_booking_response* response;
	char selected_booking_code[INT32DSTR_LEN];
	clear_screen();
	puts(title);
	get_input("Enter booking code: ", selected_booking_code, false);
	request.booking_code = atoi(selected_booking_code);
	if (cms_cancel_booking(cms, request, &response)) {
		printf("%s", response->error_message);
	}
	printf("\nBooking canceled, refund has been made.\n");
	cms_destroy_response(response);
	return 0;
}
