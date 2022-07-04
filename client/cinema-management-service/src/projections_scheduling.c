#include <cms/projections_scheduling.h>

#include "cms_operation.h"

extern struct cms_response cms_get_projections(cms_t cms, CMS_OUT struct cms_projection_details** projections) {
	struct cms_result_metadata result_metadata = {
		.ptr = (void**)projections,
		.size = sizeof(struct cms_projection_details),
		.offset_size_bitmap = (struct cms_result_bitmap[]) {
			CMS_RESULT_BITMAP_INFO(struct cms_projection_details, date, cinema_id, cinema_address, hall_number),
			CMS_RESULT_BITMAP_INFO(struct cms_projection_details, film_id, film_name, price, start_time, duration),
			CMS_RESULT_BITMAP_INFO(struct cms_projection_details, projectionist_id, projectionist_name, projectionist_surname)
		}
	};
	return cms_operation_execute(cms, GET_PROJECTION, NULL, &result_metadata);
}

extern struct cms_response cms_get_movies(cms_t cms, CMS_OUT struct cms_movie** movies) {
	struct cms_result_metadata result_metadata = {
		.ptr = (void**)movies,
		.size = sizeof(struct cms_movie),
		.offset_size_bitmap = (struct cms_result_bitmap[]) {
			CMS_RESULT_BITMAP_INFO(struct cms_movie, film_id, name, duration, studio, cast)
		}
	};
	return cms_operation_execute(cms, GET_MOVIES, NULL, &result_metadata);
}

extern struct cms_response cms_add_projection(cms_t cms, CMS_IN const struct cms_add_projection_details* details) {
	struct cms_request_param request_params[] = {
			CMS_REQUEST_PARAM_INFO(details->cinema_id),
			CMS_REQUEST_PARAM_INFO(details->hall_number),
			CMS_REQUEST_PARAM_INFO(details->date),
			CMS_REQUEST_PARAM_INFO(details->start_time),
			CMS_REQUEST_PARAM_INFO(details->price),
			CMS_REQUEST_PARAM_INFO(details->film_id)
	};
	return cms_operation_execute(cms, ADD_PROJECTION, &request_params, NULL);
}

extern struct cms_response cms_delete_projection(cms_t cms, CMS_IN const struct cms_projection* projection) {
	struct cms_request_param request_params[] = {
			CMS_REQUEST_PARAM_INFO(projection->cinema_id),
			CMS_REQUEST_PARAM_INFO(projection->hall_number),
			CMS_REQUEST_PARAM_INFO(projection->date),
			CMS_REQUEST_PARAM_INFO(projection->start_time)
	};
	return cms_operation_execute(cms, DELETE_PROJECTION, &request_params, NULL);
}

extern struct cms_response cms_get_available_projectionists(cms_t cms, CMS_IN const struct cms_projection* projection, CMS_OUT struct cms_available_projectionist** projectionists) {
	struct cms_request_param request_params[] = {
			CMS_REQUEST_PARAM_INFO(projection->cinema_id),
			CMS_REQUEST_PARAM_INFO(projection->hall_number),
			CMS_REQUEST_PARAM_INFO(projection->date),
			CMS_REQUEST_PARAM_INFO(projection->start_time)
	};
	struct cms_result_metadata result_metadata = {
		.ptr = (void**)projectionists,
		.size = sizeof(struct cms_available_projectionist),
		.offset_size_bitmap = (struct cms_result_bitmap[]) {
			CMS_RESULT_BITMAP_INFO(struct cms_available_projectionist, id, name, surname)
		}
	};
	return cms_operation_execute(cms, GET_AVAILABLE_PROJECTIONISTS, &request_params, &result_metadata);
}

extern struct cms_response cms_assign_projectionist(cms_t cms, CMS_IN const struct cms_assign_projectionist_details* details) {
	struct cms_request_param request_params[] = {
			CMS_REQUEST_PARAM_INFO(details->projectionist_id),
			CMS_REQUEST_PARAM_INFO(details->cinema_id),
			CMS_REQUEST_PARAM_INFO(details->hall_number),
			CMS_REQUEST_PARAM_INFO(details->date),
			CMS_REQUEST_PARAM_INFO(details->start_time)
	};
	return cms_operation_execute(cms, ASSIGN_PROJECTIONIST, &request_params, NULL);
}
