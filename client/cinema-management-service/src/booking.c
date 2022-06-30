#include <cms/booking.h>

#include "cms_operation.h"

extern int cms_get_cinema_screenings(cms_t cms, int32_t cinema_id, struct cms_get_cinema_screenings_response** response) {
	struct cms_request_param request_params[] = {
		CMS_REQUEST_PARAM_INFO(cinema_id)
	};
	struct cms_result_bitmap result_bitmap[] = {
		CMS_RESULT_BITMAP_INFO(struct cms_get_cinema_screenings_response, result[0]),
		CMS_RESULT_BITMAP_INFO(struct cms_screening_details, date, start_time, cinema_id, hall_number, price, film_name, running_time, film_studio, cast)
	};
	return cms_operation_execute(cms, GET_CINEMA_SCREENINGS, request_params, (struct cms_response**)response, result_bitmap);
}

extern int cms_get_available_seats(cms_t cms, struct cms_screening* screening, struct cms_get_available_seats_response** response) {
	struct cms_request_param request_params[] = {
		CMS_REQUEST_PARAM_INFO(screening->cinema_id),
		CMS_REQUEST_PARAM_INFO(screening->hall_number),
		CMS_REQUEST_PARAM_INFO(screening->date),
		CMS_REQUEST_PARAM_INFO(screening->start_time)
	};
	struct cms_result_bitmap result_bitmap[] = {
		CMS_RESULT_BITMAP_INFO(struct cms_get_available_seats_response, result[0]),
		CMS_RESULT_BITMAP_INFO(struct cms_seat, row, number)
	};
	return cms_operation_execute(cms, GET_AVAILABLE_SEATS, request_params, (struct cms_response**)response, result_bitmap);
}

extern int cms_create_booking(cms_t cms, struct cms_booking_details* details, struct cms_create_booking_response** response) {
	struct cms_request_param request_params[] = {
		CMS_REQUEST_PARAM_INFO(details->cinema_id),
		CMS_REQUEST_PARAM_INFO(details->hall_number),
		CMS_REQUEST_PARAM_INFO(details->date),
		CMS_REQUEST_PARAM_INFO(details->start_time),
		CMS_REQUEST_PARAM_INFO(details->seat_row),
		CMS_REQUEST_PARAM_INFO(details->seat_number),
	};
	struct cms_result_bitmap result_bitmap[] = {
		CMS_RESULT_BITMAP_INFO(struct cms_create_booking_response, result[0]),
		CMS_RESULT_BITMAP_INFO(struct cms_booking, booking_code)
	};
	return cms_operation_execute(cms, CREATE_BOOKING, request_params, (struct cms_response**)response, result_bitmap);
}

extern int cms_commit_booking(cms_t cms, struct cms_payment_details* payment_details, struct cms_commit_booking_response** response) {
	struct cms_request_param request_params[] = {
		CMS_REQUEST_PARAM_INFO(payment_details->booking_code),
		CMS_REQUEST_PARAM_INFO(payment_details->name_on_card),
		CMS_REQUEST_PARAM_INFO(payment_details->card_number),
		CMS_REQUEST_PARAM_INFO(payment_details->expiry_date),
		CMS_REQUEST_PARAM_INFO(payment_details->security_code)
	};
	return cms_operation_execute(cms, COMMIT_BOOKING, request_params, (struct cms_response**)response, NULL);
}

extern int cms_abort_booking(cms_t cms, char (*booking_code)[CMS_BOOKING_CODE_LEN], struct cms_abort_booking_response** response) {
	struct cms_request_param request_params[] = {
		CMS_REQUEST_PARAM_INFO(*booking_code),
	};
	return cms_operation_execute(cms, ABORT_BOOKING, request_params, (struct cms_response**)response, NULL);
}

extern int cms_cancel_booking(cms_t cms, char (*booking_code)[CMS_BOOKING_CODE_LEN], struct cms_cancel_booking_response** response) {
	struct cms_request_param request_params[] = {
		CMS_REQUEST_PARAM_INFO(*booking_code)
	};
	return cms_operation_execute(cms, CANCEL_BOOKING, request_params, (struct cms_response**)response, NULL);
}

extern int cms_validate_booking(cms_t cms, char (*booking_code)[CMS_BOOKING_CODE_LEN], struct cms_validate_booking_response** response) {
	struct cms_request_param request_params[] = {
		CMS_REQUEST_PARAM_INFO(*booking_code)
	};
	return cms_operation_execute(cms, VALIDATE_BOOKING, request_params, (struct cms_response**)response, NULL);
}
