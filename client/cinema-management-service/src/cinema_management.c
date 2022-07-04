#include <cms/cinema_management.h>

#include "cms_operation.h"

extern struct cms_response cms_get_all_cinema(cms_t cms, CMS_OUT struct cms_cinema** cinema) {
	struct cms_result_metadata result_metadata = {
		.ptr = (void**)cinema,
		.size = sizeof(struct cms_cinema),
		.offset_size_bitmap = (struct cms_result_bitmap[]) {
		CMS_RESULT_BITMAP_INFO(struct cms_cinema, id, address, opening_time, closing_time)
		}
	};
	return cms_operation_execute(cms, GET_ALL_CINEMA, NULL, &result_metadata);
}

extern struct cms_response cms_add_cinema(cms_t cms, CMS_IN const struct cms_cinema_details* details) {
	struct cms_request_param request_params[] = {
			CMS_REQUEST_PARAM_INFO(details->address),
			CMS_REQUEST_PARAM_INFO(details->opening_time),
			CMS_REQUEST_PARAM_INFO(details->closing_time)
	};
	return cms_operation_execute(cms, ADD_CINEMA, &request_params, NULL);
}

extern struct cms_response cms_delete_cinema(cms_t cms, CMS_IN int32_t cinema_id) {
	struct cms_request_param request_params[] = {
			CMS_REQUEST_PARAM_INFO(cinema_id)
	};
	return cms_operation_execute(cms, DELETE_CINEMA, &request_params, NULL);
}

extern struct cms_response cms_get_cinema_halls(cms_t cms, CMS_IN int32_t cinema_id, CMS_OUT struct cms_hall** halls) {
	struct cms_request_param request_params[] = {
			CMS_REQUEST_PARAM_INFO(cinema_id)
	};
	struct cms_result_metadata result_metadata = {
		.ptr = (void**)halls,
		.size = sizeof(struct cms_hall),
		.offset_size_bitmap = (struct cms_result_bitmap[]) {
		CMS_RESULT_BITMAP_INFO(struct cms_hall, id, num_rows, num_cols)
		}
	};
	return cms_operation_execute(cms, GET_HALLS, &request_params, &result_metadata);
}

extern struct cms_response cms_add_hall(cms_t cms, CMS_IN const struct cms_hall_details* details) {
	struct cms_request_param request_params[] = {
			CMS_REQUEST_PARAM_INFO(details->cinema_id),
			CMS_REQUEST_PARAM_INFO(details->hall_number),
			CMS_REQUEST_PARAM_INFO(details->rows),
			CMS_REQUEST_PARAM_INFO(details->rows_seats)
	};
	return cms_operation_execute(cms, ADD_HALL, &request_params, NULL);
}

extern struct cms_response cms_delete_hall(cms_t cms, CMS_IN int32_t cinema_id, CMS_IN int32_t hall_number) {
	struct cms_request_param request_params[] = {
			CMS_REQUEST_PARAM_INFO(cinema_id),
			CMS_REQUEST_PARAM_INFO(hall_number)
	};
	return cms_operation_execute(cms, DELETE_HALL, &request_params, NULL);
}
