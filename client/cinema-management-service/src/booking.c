#include <cms/booking.h>

#include "cms-operation.h"

extern int cms_get_cinema_screenings(cms_t cms, struct cms_get_cinema_screenings_request request, struct cms_get_cinema_screenings_response** response) {
	struct cms_request_param request_params[] = {
		CMS_REQUEST_PARAM_INFO(request.cinema_id)
	};
	struct cms_result_bitmap result_bitmap[] = {
		CMS_RESULT_BITMAP_INFO(struct cms_get_cinema_screenings_response, result[0]),
		CMS_RESULT_BITMAP_INFO(struct cms_screening, date, time, cinema_id, hall_id, price, film_name, running_time, film_studio, cast)
	};
	return cms_operation_execute(cms, GET_CINEMA_SCREENINGS, request_params, (struct cms_response**)response, result_bitmap);
}

extern int cms_get_available_seats(cms_t cms, struct cms_get_available_seats_request request, struct cms_get_available_seats_response** response) {
	struct cms_request_param request_params[] = {
		CMS_REQUEST_PARAM_INFO(request.cinema_id),
		CMS_REQUEST_PARAM_INFO(request.hall_id),
		CMS_REQUEST_PARAM_INFO(request.date),
		CMS_REQUEST_PARAM_INFO(request.start_time)
	};
	struct cms_result_bitmap result_bitmap[] = {
		CMS_RESULT_BITMAP_INFO(struct cms_get_available_seats_response, result[0]),
		CMS_RESULT_BITMAP_INFO(struct cms_seat, row, number)
	};
	return cms_operation_execute(cms, GET_AVAILABLE_SEATS, request_params, (struct cms_response**)response, result_bitmap);
}

extern int cms_book_seat(cms_t cms, struct cms_book_seat_request request, struct cms_book_seat_response** response) {
	struct cms_request_param request_params[] = {
		CMS_REQUEST_PARAM_INFO(request.cinema_id),
		CMS_REQUEST_PARAM_INFO(request.hall_id),
		CMS_REQUEST_PARAM_INFO(request.date),
		CMS_REQUEST_PARAM_INFO(request.start_time),
		CMS_REQUEST_PARAM_INFO(request.seat_row),
		CMS_REQUEST_PARAM_INFO(request.seat_number),
		CMS_REQUEST_PARAM_INFO(request.name_on_card),
		CMS_REQUEST_PARAM_INFO(request.card_number),
		CMS_REQUEST_PARAM_INFO(request.expiry_date),
		CMS_REQUEST_PARAM_INFO(request.security_code)
	};
	struct cms_result_bitmap result_bitmap[] = {
		CMS_RESULT_BITMAP_INFO(struct cms_book_seat_response, result[0]),
		CMS_RESULT_BITMAP_INFO(struct cms_book_seat_result, booking_code)
	};
	return cms_operation_execute(cms, BOOK_SEAT, request_params, (struct cms_response**)response, result_bitmap);
}

extern int cms_cancel_booking(cms_t cms, struct cms_cancel_booking_request request, struct cms_cancel_booking_response** response) {
	struct cms_request_param request_params[] = {
		CMS_REQUEST_PARAM_INFO(request.booking_code)
	};
	return cms_operation_execute(cms, CANCEL_BOOKING, request_params, (struct cms_response**)response, NULL);
}

extern int cms_validate_booking(cms_t cms, struct cms_validate_booking_request request, struct cms_validate_booking_response** response) {
	struct cms_request_param request_params[] = {
		CMS_REQUEST_PARAM_INFO(request.booking_code)
	};
	return cms_operation_execute(cms, VALIDATE_BOOKING, request_params, (struct cms_response**)response, NULL);
}
