#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>

#include <buracchi/common/utilities/utilities.h>
#include <buracchi/common/utilities/try.h>
#include <fort.h>
#include <cms/booking.h>

#include "resources.h"
#include "utilities/io.h"
#include "utilities/strto.h"

#define INT32DSTR_LEN 12

static void get_booking_code_or_exit_choise(int32_t* booking_code, bool* exit);

extern int cancel_booking(cms_t cms) {
	struct cms_cancel_booking_request request = { 0 };
	struct cms_cancel_booking_response* response;
	bool exit = false;
	while (!exit) {
		get_booking_code_or_exit_choise(&request.booking_code, &exit);
		if (exit) {
			return 0;
		}
		try(cms_cancel_booking(cms, request, &response), true, fail);
		if (!response->error_message) {
			printf("\nBooking canceled, refund has been made.\n");
			cms_destroy_response((struct cms_response*)response);
			press_anykey();
			exit = true;
		}
		else {
			char options[2] = { 'Y', 'N' };
			char choise;
			printf("%s\n", response->error_message);
			choise = multi_choice("Do you want to try insert another code?", options, 2);
			if (choise == 'N') {
				exit = true;
			}
		}
	}
	return 0;
fail:
	cms_destroy_response((struct cms_response*)response);
	return 1;
}

static void get_booking_code_or_exit_choise(int32_t* booking_code, bool* exit) {
	char input[INT32DSTR_LEN] = { 0 };
	bool valid_input = false;
	while (!valid_input) {
		io_clear_screen();
		puts(title);
		get_input("Enter booking code or enter Q tu get back: ", input, false);
		*exit = (input[0] == 'Q' && input[1] == '\0');
		valid_input = *exit || (strtoint32(booking_code, input, 10) == STRTO_SUCCESS);
	}
}
