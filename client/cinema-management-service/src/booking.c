#include <cms/booking.h>

#include "cms_operation.h"

extern struct cms_response cms_get_cinema_screenings(cms_t cms, CMS_IN int32_t cinema_id, CMS_OUT struct cms_screening_details** screening_details) {
	struct cms_request_param request_params[] = {
		CMS_REQUEST_PARAM_INFO(cinema_id)
	};
	struct cms_result_metadata result_metadata = {
		.ptr = (void**)screening_details,
		.size = sizeof(struct cms_screening_details),
		.offset_size_bitmap = (struct cms_result_bitmap[]) {
		CMS_RESULT_BITMAP_INFO(struct cms_screening_details, date, start_time, cinema_id, hall_number, price),
		CMS_RESULT_BITMAP_INFO(struct cms_screening_details, film_name, running_time, film_studio, cast)
		}
	};
	return cms_operation_execute(cms, GET_CINEMA_SCREENINGS, &request_params, &result_metadata);
}

extern struct cms_response cms_get_available_seats(cms_t cms, CMS_IN const struct cms_screening* screening, CMS_OUT struct cms_seat** seats) {
	struct cms_request_param request_params[] = {
		CMS_REQUEST_PARAM_INFO(screening->cinema_id),
		CMS_REQUEST_PARAM_INFO(screening->hall_number),
		CMS_REQUEST_PARAM_INFO(screening->date),
		CMS_REQUEST_PARAM_INFO(screening->start_time)
	};
	struct cms_result_metadata result_metadata = {
		.ptr = (void**)seats,
		.size = sizeof(struct cms_seat),
		.offset_size_bitmap = (struct cms_result_bitmap[]) {
			CMS_RESULT_BITMAP_INFO(struct cms_seat, row, number)
		}
	};
	return cms_operation_execute(cms, GET_AVAILABLE_SEATS, &request_params, &result_metadata);
}

extern struct cms_response cms_create_booking(cms_t cms, CMS_IN const struct cms_booking_details* details, CMS_OUT char(**booking_code)[CMS_BOOKING_CODE_LEN]) {
	struct cms_request_param request_params[] = {
		CMS_REQUEST_PARAM_INFO(details->cinema_id),
		CMS_REQUEST_PARAM_INFO(details->hall_number),
		CMS_REQUEST_PARAM_INFO(details->date),
		CMS_REQUEST_PARAM_INFO(details->start_time),
		CMS_REQUEST_PARAM_INFO(details->seat_row),
		CMS_REQUEST_PARAM_INFO(details->seat_number),
	};
	struct cms_result_metadata result_metadata = {
		.ptr = (void**)booking_code,
		.size = CMS_BOOKING_CODE_LEN,
		.offset_size_bitmap = (struct cms_result_bitmap[]) {
			{ 0, CMS_BOOKING_CODE_LEN }
		}
	};
	return cms_operation_execute(cms, CREATE_BOOKING, &request_params, &result_metadata);
}

extern struct cms_response cms_commit_booking(cms_t cms, CMS_IN const struct cms_payment_details* payment_details) {
	struct cms_request_param request_params[] = {
		CMS_REQUEST_PARAM_INFO(payment_details->booking_code),
		CMS_REQUEST_PARAM_INFO(payment_details->name_on_card),
		CMS_REQUEST_PARAM_INFO(payment_details->card_number),
		CMS_REQUEST_PARAM_INFO(payment_details->expiry_date),
		CMS_REQUEST_PARAM_INFO(payment_details->security_code)
	};
	return cms_operation_execute(cms, COMMIT_BOOKING, &request_params, NULL);
}

extern struct cms_response cms_abort_booking(cms_t cms, CMS_IN const char(*booking_code)[CMS_BOOKING_CODE_LEN]) {
	struct cms_request_param request_params[] = {
		CMS_REQUEST_PARAM_INFO(*booking_code),
	};
	return cms_operation_execute(cms, ABORT_BOOKING, &request_params, NULL);
}

extern struct cms_response cms_cancel_booking(cms_t cms, CMS_IN const char(*booking_code)[CMS_BOOKING_CODE_LEN]) {
	struct cms_request_param request_params[] = {
		CMS_REQUEST_PARAM_INFO(*booking_code)
	};
	return cms_operation_execute(cms, CANCEL_BOOKING, &request_params, NULL);
}

extern struct cms_response cms_validate_booking(cms_t cms, CMS_IN const char(*booking_code)[CMS_BOOKING_CODE_LEN]) {
	struct cms_request_param request_params[] = {
		CMS_REQUEST_PARAM_INFO(*booking_code)
	};
	return cms_operation_execute(cms, VALIDATE_BOOKING, &request_params, NULL);
}
