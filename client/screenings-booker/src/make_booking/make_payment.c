#include "../make_booking.h"

#include <stdio.h>
#include <stdbool.h>

#include <buracchi/common/utilities/utilities.h>
#include <buracchi/common/utilities/try.h>
#include <cms/booking.h>
#include <cliutils/io.h>

#include "../core.h"

extern int make_payment(cms_t cms, struct booking_data* booking_data) {
	struct cms_book_seat_request request = {
		.cinema_id = booking_data->cinema_id,
		.hall_id = booking_data->hall,
		.seat_row = booking_data->seat_row,
		.seat_number = (int32_t)booking_data->seat_number
	};
	strcpy((char*)request.date, booking_data->date);
	strcpy((char*)request.start_time, booking_data->time);
	struct cms_book_seat_response* response;
	io_clear_screen();
	puts(title);
	printf("Dettagli ordine\n\n");
	printf("Data: %s\n", booking_data->date);
	printf("Orario: %s\n", booking_data->time);
	printf("Cinema: %s\n", booking_data->cinema_address);
	printf("Sala: %d\n", booking_data->hall);
	printf("Film: %s\n", booking_data->film_name);
	printf("Posto: %c%u\n", booking_data->seat_row, booking_data->seat_number);
	printf("Prezzo: %s\n\n", booking_data->price);
	if (multi_choice("Procedere al pagamento?", ((char[2]){ 'S', 'N' })) == 'N') {
		return choose_seat(cms, booking_data);
	}
	puts("");
    get_input("Intestatario carta: ", request.name_on_card, false);
	get_input("Numero carta: ", request.card_number, false);
	get_input("Data di scadenza [YYYY-MM]: ", request.expiry_date, false);
	get_input("Codice di sicurezza (CVV): ", request.security_code, true);
	try(cms_book_seat(cms, request, &response), 1, fail);
	if (response->error_message) {
		printf("%s", response->error_message);
		cms_destroy_response((struct cms_response*)response);
		press_anykey();
		return 0;
	}
	booking_data->booking_code = response->result[0].booking_code;
	cms_destroy_response((struct cms_response*)response);
	printf("\nIl codice di prenotazione e': %d\n", booking_data->booking_code);
	puts("Il codice di prenotazione e' necessario per accedere al cinema o per effettuare il rimborso.");
	puts("Salvarlo in un luogo sicuro ed assicurarsi di non perderlo.");
	press_anykey();
	return 0;
fail:
	return 1;
}
