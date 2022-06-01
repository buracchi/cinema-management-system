#include "booking.h"

#include "cms-statement.h"

extern bool cms_get_all_cinema(cms_t cms, struct cms_get_all_cinema_response** response) {
	struct cms_result_bitmap result_bitmap[5] = {
		CMS_RESULT_BITMAP_INFO(struct cms_get_all_cinema_response, result[0]),
		CMS_RESULT_BITMAP_INFO(struct cms_cinema, id, address, opening_time, closing_time)
	};
	return cms_operation_execute(cms, GET_ALL_CINEMA, NULL, response, result_bitmap);
}

extern bool cms_get_cinema_screenings(cms_t cms, struct cms_get_cinema_screenings_request request, struct cms_get_cinema_screenings_response** response) {
	struct cms_request_param request_params[1] = { { &request.cinema_id,	sizeof(request.cinema_id) } };
	struct cms_result_bitmap result_bitmap[10] = {
		CMS_RESULT_BITMAP_INFO(struct cms_get_cinema_screenings_response, result[0]),
		CMS_RESULT_BITMAP_INFO(struct cms_screening, date, time, cinema_id, hall_id, price, film_name, running_time, film_studio, cast)
	};
	return cms_operation_execute(cms, GET_CINEMA_SCREENINGS, request_params, response, result_bitmap);
}

extern bool cms_get_cinema_halls(cms_t cms, struct cms_get_cinema_halls_request request, struct cms_get_cinema_halls_response** response) {
	struct cms_request_param request_params[1] = { { &request.cinema_id,	sizeof(request.cinema_id) } };
	struct cms_result_bitmap result_bitmap[4] = {
		CMS_RESULT_BITMAP_INFO(struct cms_get_cinema_halls_response, result[0]),
		CMS_RESULT_BITMAP_INFO(struct cms_hall_info, id, num_rows, num_cols)
	};
	return cms_operation_execute(cms, GET_HALLS, request_params, response, result_bitmap);
}

extern bool cms_get_available_seats(cms_t cms, struct cms_get_available_seats_request request, struct cms_get_available_seats_response** response) {
	struct cms_request_param request_params[4] = {
		{ &request.cinema_id,	sizeof(request.cinema_id) },
		{ &request.hall_id,		sizeof(request.cinema_id) },
		{ &request.date,		sizeof(request.date) },
		{ &request.start_time,	sizeof(request.start_time) }
	};
	struct cms_result_bitmap result_bitmap[3] = {
		CMS_RESULT_BITMAP_INFO(struct cms_get_available_seats_response, result[0]),
		CMS_RESULT_BITMAP_INFO(struct cms_seat, row, number)
	};
	return cms_operation_execute(cms, GET_AVALILABLE_SEATS, request_params, response, result_bitmap);
}

extern bool cms_book_seat(cms_t cms, struct cms_book_seat_request request, struct cms_book_seat_response** response) {
	struct cms_request_param request_params[10] = {
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
	struct cms_result_bitmap result_bitmap[3] = {
		CMS_RESULT_BITMAP_INFO(struct cms_book_seat_response, result[0]),
		CMS_RESULT_BITMAP_INFO(struct cms_book_seat_result, booking_code)
	};
	return cms_operation_execute(cms, BOOK_SEAT, request_params, response, result_bitmap);
}

extern bool cms_cancel_booking(cms_t cms, struct cms_cancel_booking_request request, struct cms_cancel_booking_response** response) {
	struct cms_request_param request_params[1] = { { &request.booking_code,	sizeof(request.booking_code) } };
	return cms_operation_execute(cms, CANCEL_BOOKING, request_params, response, NULL);
}

extern bool cms_validate_booking(cms_t cms, struct cms_validate_booking_request request, struct cms_validate_booking_response** response) {
	struct cms_request_param request_params[1] = { { &request.booking_code,	sizeof(request.booking_code) } };
	return cms_operation_execute(cms, VALIDATE_BOOKING, request_params, response, NULL);
}
