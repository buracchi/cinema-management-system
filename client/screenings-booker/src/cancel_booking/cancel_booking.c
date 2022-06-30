#include "cancel_booking.h"

#include <stdio.h>
#include <stdbool.h>

#include <buracchi/common/utilities/utilities.h>
#include <buracchi/common/utilities/strto.h>
#include <buracchi/common/utilities/try.h>
#include <cms/booking.h>
#include <cliutils/io.h>

#include "../core.h"

extern int cancel_booking(cms_t cms) {
	char booking_code[CMS_BOOKING_CODE_LEN] = { 0 };
	struct cms_cancel_booking_response* response = NULL;
	char input[CMS_BOOKING_CODE_LEN] = { 0 };
	while (true) {
		while (true) {
			io_clear_screen();
			puts(title);
			get_input("Inserire il codice di prenotazione da annullare o Q per tornare indietro: ", input, false);
			if (input[0] == 'Q' && input[1] == '\0') {
				return 0;
			}
			if ((strlen(input) == sizeof booking_code - 1) && cmn_strto_uint32(&(uint32_t){ 0 }, input, 16) == 0) {
				strcpy(booking_code, input);
				break;
			}
		}
		puts("");
		try(cms_cancel_booking(cms, &booking_code, &response), 1, fail);
		if (response->error_message) {
			printf("%s\n", response->error_message);
			cms_destroy_response((struct cms_response*)response);
			puts("");
			if (multi_choice("Riprovare con un altro codice?", ((char[]){ 'S', 'N' })) == 'S') {
				continue;
			}
			return 0;
		}
		cms_destroy_response((struct cms_response*)response);
		printf("Prenotazione annullata, e' stato effettuato con successo il rimborso.\n");
		press_anykey();
		return 0;
	}
fail:
	if (response) {
		if (response->error_message) {
			fprintf(stderr, "%s\n", response->error_message);
		}
		cms_destroy_response((struct cms_response*)response);
	}
	return 1;
}
