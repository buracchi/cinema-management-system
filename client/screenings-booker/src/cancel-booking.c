#include <stdio.h>
#include <stdbool.h>

#include <buracchi/common/utilities/utilities.h>
#include <buracchi/common/utilities/try.h>
#include <cms/booking.h>

#include "resources.h"
#include "utilities/io.h"
#include "utilities/strto.h"

#define INT32DSTR_LEN 12

extern int cancel_booking(cms_t cms) {
	struct cms_cancel_booking_request request = { 0 };
	struct cms_cancel_booking_response* response;
	char input[INT32DSTR_LEN] = { 0 };
	while (true) {
		while (true) {
			io_clear_screen();
			puts(title);
			get_input("Inserire il codice di prenotazione da annullare o Q per tornare indietro: ", input, false);
			if ((input[0] == 'Q' || input[0] == 'q') && input[1] == '\0') {
				return 0;
			}
			if (strtoint32(&(request.booking_code), input, 10) == STRTO_SUCCESS) {
				break;
			}
		}
		try(cms_cancel_booking(cms, request, &response), 1, fail);
		if (response->error_message) {
			printf("%s\n", response->error_message);
			cms_destroy_response((struct cms_response*)response);
			if (multi_choice("Riprovare con un altro codice?", ((char[2]){ 'S', 'N' })) == 'S') {
				continue;
			}
			return 0;
		}
		cms_destroy_response((struct cms_response*)response);
		printf("\nPrenotazione annullata, e' stato effettuato con successo il rimborso.\n");
		press_anykey();
		return 0;
	}
fail:
	return 1;
}
