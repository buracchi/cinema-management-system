#include <cms/shift_scheduling.h>

#include "cms_operation.h"

extern int cms_get_shifts(cms_t cms, struct cms_get_shifts_response** response) {
	struct cms_result_bitmap result_bitmap[] = {
			CMS_RESULT_BITMAP_INFO(struct cms_get_shifts_response, result[0]),
			CMS_RESULT_BITMAP_INFO(struct cms_shift, day, start_time, duration, cinema_id, cinema_address),
			CMS_RESULT_BITMAP_INFO(struct cms_shift, employee_id, employee_name, employee_surname, employee_role)
	};
	return cms_operation_execute(cms, GET_SHIFTS, NULL, (struct cms_response**)response, result_bitmap);
}

extern int cms_add_shift(cms_t cms, struct cms_add_shift_request* request, struct cms_add_shift_response** response) {
	struct cms_request_param request_params[] = {
			CMS_REQUEST_PARAM_INFO(request->employee_id),
			CMS_REQUEST_PARAM_INFO(request->day),
			CMS_REQUEST_PARAM_INFO(request->start_time),
			CMS_REQUEST_PARAM_INFO(request->duration),
			CMS_REQUEST_PARAM_INFO(request->cinema_id)
	};
	return cms_operation_execute(cms, ADD_SHIFT, request_params, (struct cms_response**)response, NULL);
}

extern int cms_delete_shift(cms_t cms, struct cms_delete_shift_request* request, struct cms_delete_shift_response** response) {
	struct cms_request_param request_params[] = {
			CMS_REQUEST_PARAM_INFO(request->employee_id),
			CMS_REQUEST_PARAM_INFO(request->day),
			CMS_REQUEST_PARAM_INFO(request->start_time)
	};
	return cms_operation_execute(cms, DELETE_SHIFT, request_params, (struct cms_response**)response, NULL);
}
