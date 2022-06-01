#include "booking.h"

#include <stddef.h>

#include "cms-statement.h"

extern bool cms_get_all_cinema(cms_t cms, struct cms_get_all_cinema_response** response) {
	struct result_bitmap result_bitmap[5] = {
		{ offsetof(struct cms_get_all_cinema_response, result),			sizeof(struct cms_cinema) },
		{ offsetof(struct cms_cinema, id),								sizeof(((struct cms_cinema*)0)->id) },
		{ offsetof(struct cms_cinema, address),							sizeof(((struct cms_cinema*)0)->address) },
		{ offsetof(struct cms_cinema, opening_time),					sizeof(((struct cms_cinema*)0)->opening_time) },
		{ offsetof(struct cms_cinema, closing_time),					sizeof(((struct cms_cinema*)0)->closing_time) }
	};
	return cms_stmt_execute(cms, GET_ALL_CINEMA, NULL, response, result_bitmap);
}

extern bool cms_get_cinema_screenings(cms_t cms, struct cms_get_cinema_screenings_request request, struct cms_get_cinema_screenings_response** response) {
	struct request_param request_params[1] = { { &request.cinema_id,	sizeof(request.cinema_id) } };
	struct result_bitmap result_bitmap[10] = {
		{ offsetof(struct cms_get_cinema_screenings_response, result),	sizeof(struct cms_screening) },
		{ offsetof(struct cms_screening, date),							sizeof(((struct cms_screening*)0)->date) },
		{ offsetof(struct cms_screening, time),							sizeof(((struct cms_screening*)0)->time) },
		{ offsetof(struct cms_screening, cinema_id),					sizeof(((struct cms_screening*)0)->cinema_id) },
		{ offsetof(struct cms_screening, hall_id),						sizeof(((struct cms_screening*)0)->hall_id) },
		{ offsetof(struct cms_screening, price),						sizeof(((struct cms_screening*)0)->price) },
		{ offsetof(struct cms_screening, film_name),					sizeof(((struct cms_screening*)0)->film_name) },
		{ offsetof(struct cms_screening, running_time),					sizeof(((struct cms_screening*)0)->running_time) },
		{ offsetof(struct cms_screening, film_studio),					sizeof(((struct cms_screening*)0)->film_studio) },
		{ offsetof(struct cms_screening, cast),							sizeof(((struct cms_screening*)0)->cast) }
	};
	return cms_stmt_execute(cms, GET_CINEMA_SCREENINGS, request_params, response, result_bitmap);
}

extern bool cms_get_cinema_halls(cms_t cms, struct cms_get_cinema_halls_request request, struct cms_get_cinema_halls_response** response) {
	struct request_param request_params[1] = { { &request.cinema_id,	sizeof(request.cinema_id) } };
	struct result_bitmap result_bitmap[4] = {
		{ offsetof(struct cms_get_cinema_halls_response, result),		sizeof(struct cms_hall_info) },
		{ offsetof(struct cms_hall_info, id),							sizeof(((struct cms_hall_info*)0)->id) },
		{ offsetof(struct cms_hall_info, num_rows),						sizeof(((struct cms_hall_info*)0)->num_rows) },
		{ offsetof(struct cms_hall_info, num_cols),						sizeof(((struct cms_hall_info*)0)->num_cols) }
	};
	return cms_stmt_execute(cms, GET_HALLS, request_params, response, result_bitmap);
}

extern bool cms_get_available_seats(cms_t cms, struct cms_get_available_seats_request request, struct cms_get_available_seats_response** response) {
	struct request_param request_params[4] = {
		{ &request.cinema_id,	sizeof(request.cinema_id) },
		{ &request.hall_id,		sizeof(request.cinema_id) },
		{ &request.date,		sizeof(request.date) },
		{ &request.start_time,	sizeof(request.start_time) }
	};
	struct result_bitmap result_bitmap[3] = {
		{ offsetof(struct cms_get_available_seats_response, result),	sizeof(struct cms_seat) },
		{ offsetof(struct cms_seat, row),								sizeof(((struct cms_seat*)0)->row) },
		{ offsetof(struct cms_seat, number),							sizeof(((struct cms_seat*)0)->number) }
	};
	return cms_stmt_execute(cms, GET_AVALILABLE_SEATS, request_params, response, result_bitmap);
}

extern bool cms_book_seat(cms_t cms, struct cms_book_seat_request request, struct cms_book_seat_response** response) {
	struct request_param request_params[10] = {
		{ &request.cinema_id,		sizeof(request.cinema_id) },
		{ &request.hall_id,			sizeof(request.hall_id) },
		{ &request.date,			sizeof(request.date) },
		{ &request.start_time,		sizeof(request.start_time) },
		{ &request.seat_row,		sizeof(request.seat_row) },
		{ &request.seat_number,		sizeof(request.seat_number) },
		{ &request.name_on_card,	sizeof(request.name_on_card) },
		{ &request.card_number,		sizeof(request.card_number) },
		{ &request.expiry_date,		sizeof(request.expiry_date) },
		{ &request.security_code,	sizeof(request.security_code) }
	};
	struct result_bitmap result_bitmap[3] = {
		{ offsetof(struct cms_book_seat_response, result),				sizeof(struct cms_book_seat_result) },
		{ offsetof(struct cms_book_seat_result, booking_code),			sizeof(((struct cms_book_seat_result*)0)->booking_code) },
	};
	return cms_stmt_execute(cms, BOOK_SEAT, request_params, response, result_bitmap);
}

extern bool cms_cancel_booking(cms_t cms, struct cms_cancel_booking_request request, struct cms_cancel_booking_response** response) {
	struct request_param request_params[1] = { { &request.booking_code,	sizeof(request.booking_code) } };
	return cms_stmt_execute(cms, CANCEL_BOOKING, request_params, response, NULL);
}

extern bool cms_validate_booking(cms_t cms, struct cms_validate_booking_request request, struct cms_validate_booking_response** response) {
	struct request_param request_params[1] = { { &request.booking_code,	sizeof(request.booking_code) } };
	return cms_stmt_execute(cms, VALIDATE_BOOKING, request_params, response, NULL);
}
