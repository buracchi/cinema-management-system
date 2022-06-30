#include "make_booking.h"

#include <stdio.h>
#include <stdbool.h>

#include <buracchi/common/utilities/utilities.h>
#include <buracchi/common/utilities/try.h>
#include <cms/booking.h>
#include <cliutils/io.h>

#include "../core.h"

static int create_booking(cms_t cms, struct booking_data* booking_data);
static int abort_booking(cms_t cms, struct booking_data* booking_data);

extern int make_payment(cms_t cms, struct booking_data* booking_data) {
	struct cms_payment_details payment_details = {0 };
	struct cms_commit_booking_response* response = NULL;
	switch (create_booking(cms, booking_data)) {
	case 1:
		return 1;
	case 2:
		return 0;
	}
	strcpy(payment_details.booking_code, booking_data->booking_code);
	while (true) {
		io_clear_screen();
		puts(title);
		printf("Dettagli ordine\n\n");
		printf("Data: %s\n", booking_data->date);
		printf("Orario: %s\n", booking_data->time);
		printf("Cinema: %s\n", booking_data->cinema_address);
		printf("Sala: %d\n", booking_data->hall);
		printf("Film: %s\n", booking_data->film_name);
		printf("Posto: %c%u\n", booking_data->seat_row[0], booking_data->seat_number);
		printf("Prezzo: %s\n\n", booking_data->price);
		if (multi_choice("Procedere al pagamento?", ((char[]){ 'S', 'N' })) == 'N') {
			switch (abort_booking(cms, booking_data)) {
			case 1:
				return 1;
			case 2:
				return 0;
			}
			return choose_seat(cms, booking_data);
		}
		puts("");
		get_input("Intestatario carta: ", payment_details.name_on_card, false);
		get_input("Intestatario carta: ", payment_details.name_on_card, false);
		get_input("Numero carta: ", payment_details.card_number, false);
		get_input_len("Data di scadenza [YYYY-MM]: ", 8, payment_details.expiry_date, false);
		get_input("Codice di sicurezza (CVV): ", payment_details.security_code, true);
		puts("");
		if (multi_choice("Confermare dati inseriti?", ((char[]){ 'S', 'N' })) == 'N') {
			continue;
		}
		puts("");
		((char*)payment_details.expiry_date)[7] = '-';
		((char*)payment_details.expiry_date)[8] = '0';
		((char*)payment_details.expiry_date)[9] = '1';
		((char*)payment_details.expiry_date)[10] = '\0';
		try(cms_commit_booking(cms, &payment_details, &response), 1, fail);
		if (response->error_message) {
			printf("%s\n", response->error_message);
			cms_destroy_response((struct cms_response*)response);
			press_anykey();
			return 0;
		}
		cms_destroy_response((struct cms_response*)response);
		printf("Il codice di prenotazione e': %s\n", booking_data->booking_code);
		puts("");
		puts("Il codice di prenotazione e' necessario per accedere al cinema o per effettuare il rimborso.");
		puts("Salvarlo in un luogo sicuro ed assicurarsi di non perderlo.");
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

static int create_booking(cms_t cms, struct booking_data* booking_data) {
	struct cms_booking_details details = {
		.cinema_id = booking_data->cinema_id,
		.hall_number = booking_data->hall,
		.seat_number = (int32_t)booking_data->seat_number
	};
	strcpy((char*)details.seat_row, booking_data->seat_row);
	strcpy((char*)details.date, booking_data->date);
	strcpy((char*)details.start_time, booking_data->time);
	struct cms_create_booking_response* response = NULL;
	try(cms_create_booking(cms, &details, &response), 1, fail);
	if (response->error_message) {
		printf("%s\n", response->error_message);
		cms_destroy_response((struct cms_response*)response);
		press_anykey();
		return 2;
	}
	strcpy(booking_data->booking_code, response->result[0].booking_code);
	cms_destroy_response((struct cms_response*)response);
	return 0;
fail:
	if (response) {
		if (response->error_message) {
			fprintf(stderr, "%s\n", response->error_message);
		}
		cms_destroy_response((struct cms_response*)response);
	}
	return 1;
}

static int abort_booking(cms_t cms, struct booking_data* booking_data) {
	struct cms_abort_booking_response* response = NULL;
	try(cms_abort_booking(cms, &booking_data->booking_code, &response), 1, fail);
	if (response->error_message) {
		printf("%s\n", response->error_message);
		cms_destroy_response((struct cms_response*)response);
		press_anykey();
		return 2;
	}
	cms_destroy_response((struct cms_response*)response);
	return 0;
fail:
	if (response) {
		if (response->error_message) {
			fprintf(stderr, "%s\n", response->error_message);
		}
		cms_destroy_response((struct cms_response*)response);
	}
	return 1;
}
