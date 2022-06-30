#include <cms/projections_scheduling.h>

#include "cms_operation.h"

extern int cms_get_projections(cms_t cms, struct cms_get_projections_response** response) {
	struct cms_result_bitmap result_bitmap[] = {
			CMS_RESULT_BITMAP_INFO(struct cms_get_projections_response, result[0]),
			CMS_RESULT_BITMAP_INFO(struct cms_projection, date, cinema_id, cinema_address, hall_number),
			CMS_RESULT_BITMAP_INFO(struct cms_projection, film_id, film_name, price, start_time, duration),
			CMS_RESULT_BITMAP_INFO(struct cms_projection, projectionist_id, projectionist_name, projectionist_surname)
	};
	return cms_operation_execute(cms, GET_PROJECTION, NULL, (struct cms_response**)response, result_bitmap);
}

extern int cms_get_movies(cms_t cms, struct cms_get_movies_response** response) {
	struct cms_result_bitmap result_bitmap[] = {
			CMS_RESULT_BITMAP_INFO(struct cms_get_movies_response, result[0]),
			CMS_RESULT_BITMAP_INFO(struct cms_movie, film_id, name, duration, studio, cast),
	};
	return cms_operation_execute(cms, GET_MOVIES, NULL, (struct cms_response**)response, result_bitmap);
}

extern int cms_add_projection(cms_t cms, struct cms_add_projection_request* request, struct cms_add_projection_response** response) {
	struct cms_request_param request_params[] = {
			CMS_REQUEST_PARAM_INFO(request->cinema_id),
			CMS_REQUEST_PARAM_INFO(request->hall_number),
			CMS_REQUEST_PARAM_INFO(request->date),
			CMS_REQUEST_PARAM_INFO(request->start_time),
			CMS_REQUEST_PARAM_INFO(request->price),
			CMS_REQUEST_PARAM_INFO(request->film_id)
	};
	return cms_operation_execute(cms, ADD_PROJECTION, request_params, (struct cms_response**)response, NULL);
}

extern int cms_delete_projection(cms_t cms, struct cms_delete_projection_request* request, struct cms_delete_projection_response** response) {
	struct cms_request_param request_params[] = {
			CMS_REQUEST_PARAM_INFO(request->cinema_id),
			CMS_REQUEST_PARAM_INFO(request->hall_number),
			CMS_REQUEST_PARAM_INFO(request->date),
			CMS_REQUEST_PARAM_INFO(request->start_time)
	};
	return cms_operation_execute(cms, DELETE_PROJECTION, request_params, (struct cms_response**)response, NULL);
}

extern int cms_get_available_projectionists(cms_t cms, struct cms_get_available_projectionists_request* request, struct cms_get_available_projectionists_response** response) {
	struct cms_request_param request_params[] = {
			CMS_REQUEST_PARAM_INFO(request->cinema_id),
			CMS_REQUEST_PARAM_INFO(request->hall_number),
			CMS_REQUEST_PARAM_INFO(request->date),
			CMS_REQUEST_PARAM_INFO(request->start_time)
	};
	struct cms_result_bitmap result_bitmap[] = {
			CMS_RESULT_BITMAP_INFO(struct cms_get_available_projectionists_response, result[0]),
			CMS_RESULT_BITMAP_INFO(struct cms_available_projectionist, id, name, surname)
	};
	return cms_operation_execute(cms, GET_AVAILABLE_PROJECTIONISTS, request_params, (struct cms_response**)response, result_bitmap);
}

extern int cms_assign_projectionist(cms_t cms, struct cms_assign_projectionist_request* request, struct cms_assign_projectionist_response** response) {
	struct cms_request_param request_params[] = {
			CMS_REQUEST_PARAM_INFO(request->projectionist_id),
			CMS_REQUEST_PARAM_INFO(request->cinema_id),
			CMS_REQUEST_PARAM_INFO(request->hall_number),
			CMS_REQUEST_PARAM_INFO(request->date),
			CMS_REQUEST_PARAM_INFO(request->start_time)
	};
	return cms_operation_execute(cms, ASSIGN_PROJECTIONIST, request_params, (struct cms_response**)response, NULL);
}
