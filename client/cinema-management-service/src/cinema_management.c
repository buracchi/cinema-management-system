#include <cms/cinema_management.h>

#include "cms_operation.h"

extern int cms_get_all_cinema(cms_t cms, struct cms_get_all_cinema_response** response) {
	struct cms_result_bitmap result_bitmap[] = {
			CMS_RESULT_BITMAP_INFO(struct cms_get_all_cinema_response, result[0]),
			CMS_RESULT_BITMAP_INFO(struct cms_cinema, id, address, opening_time, closing_time)
	};
	return cms_operation_execute(cms, GET_ALL_CINEMA, NULL, (struct cms_response**)response, result_bitmap);
}

extern int cms_add_cinema(cms_t cms, struct cms_add_cinema_request* request, struct cms_add_cinema_response** response) {
	struct cms_request_param request_params[] = {
			CMS_REQUEST_PARAM_INFO(request->address),
			CMS_REQUEST_PARAM_INFO(request->opening_time),
			CMS_REQUEST_PARAM_INFO(request->closing_time)
	};
	return cms_operation_execute(cms, ADD_CINEMA, request_params, (struct cms_response**)response, NULL);
}

extern int cms_delete_cinema(cms_t cms, struct cms_delete_cinema_request* request, struct cms_delete_cinema_response** response) {
	struct cms_request_param request_params[] = {
			CMS_REQUEST_PARAM_INFO(request->id)
	};
	return cms_operation_execute(cms, DELETE_CINEMA, request_params, (struct cms_response**)response, NULL);
}

extern int cms_get_cinema_halls(cms_t cms, struct cms_get_cinema_halls_request* request, struct cms_get_cinema_halls_response** response) {
	struct cms_request_param request_params[] = {
			CMS_REQUEST_PARAM_INFO(request->cinema_id)
	};
	struct cms_result_bitmap result_bitmap[] = {
			CMS_RESULT_BITMAP_INFO(struct cms_get_cinema_halls_response, result[0]),
			CMS_RESULT_BITMAP_INFO(struct cms_hall_info, id, num_rows, num_cols)
	};
	return cms_operation_execute(cms, GET_HALLS, request_params, (struct cms_response**)response, result_bitmap);
}

extern int cms_add_hall(cms_t cms, struct cms_add_hall_request* request, struct cms_add_hall_response** response) {
	struct cms_request_param request_params[] = {
			CMS_REQUEST_PARAM_INFO(request->cinema_id),
			CMS_REQUEST_PARAM_INFO(request->hall_number),
			CMS_REQUEST_PARAM_INFO(request->rows),
			CMS_REQUEST_PARAM_INFO(request->rows_seats)
	};
	return cms_operation_execute(cms, ADD_HALL, request_params, (struct cms_response**)response, NULL);
}

extern int cms_delete_hall(cms_t cms, struct cms_delete_hall_request* request, struct cms_delete_hall_response** response) {
	struct cms_request_param request_params[] = {
			CMS_REQUEST_PARAM_INFO(request->cinema_id),
			CMS_REQUEST_PARAM_INFO(request->hall_number)
	};
	return cms_operation_execute(cms, DELETE_HALL, request_params, (struct cms_response**)response, NULL);
}
