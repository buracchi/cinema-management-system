#include <cms/shift_scheduling.h>

#include "cms_operation.h"

extern struct cms_response cms_get_shifts(cms_t cms, CMS_OUT struct cms_shift_details** shifts) {
	struct cms_result_metadata result_metadata = {
		.ptr = (void**)shifts,
		.size = sizeof(struct cms_result_bitmap),
		.offset_size_bitmap = (struct cms_result_bitmap[]) {
			CMS_RESULT_BITMAP_INFO(struct cms_shift_details, day, start_time, duration, cinema_id, cinema_address),
			CMS_RESULT_BITMAP_INFO(struct cms_shift_details, employee_id, employee_name, employee_surname, employee_role)
		}
	};
	return cms_operation_execute(cms, GET_SHIFTS, NULL, &result_metadata);
}

extern struct cms_response cms_add_shift(cms_t cms, CMS_IN const struct cms_add_shift_details* shift_details) {
	struct cms_request_param request_params[] = {
			CMS_REQUEST_PARAM_INFO(shift_details->employee_id),
			CMS_REQUEST_PARAM_INFO(shift_details->day),
			CMS_REQUEST_PARAM_INFO(shift_details->start_time),
			CMS_REQUEST_PARAM_INFO(shift_details->duration),
			CMS_REQUEST_PARAM_INFO(shift_details->cinema_id)
	};
	return cms_operation_execute(cms, ADD_SHIFT, &request_params, NULL);
}

extern struct cms_response cms_delete_shift(cms_t cms, CMS_IN const struct cms_shift* shift) {
	struct cms_request_param request_params[] = {
			CMS_REQUEST_PARAM_INFO(shift->employee_id),
			CMS_REQUEST_PARAM_INFO(shift->day),
			CMS_REQUEST_PARAM_INFO(shift->start_time)
	};
	return cms_operation_execute(cms, DELETE_SHIFT, &request_params, NULL);
}
