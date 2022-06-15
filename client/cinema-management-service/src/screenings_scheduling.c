#include <cms/screenings_scheduling.h>

#include "cms-operation.h"

extern int cms_get_screenings(cms_t cms, struct cms_get_screenings_response** response) {
	struct cms_result_bitmap result_bitmap[] = {
			CMS_RESULT_BITMAP_INFO(struct cms_get_screenings_response, result[0]),
			CMS_RESULT_BITMAP_INFO(struct cms_screenings, date, cinema_id, cinema_address, hall_number),
			CMS_RESULT_BITMAP_INFO(struct cms_screenings, film_id, film_name, price, start_time, duration),
			CMS_RESULT_BITMAP_INFO(struct cms_screenings, projectionist_id, projectionist_name, projectionist_surname)
	};
	return cms_operation_execute(cms, GET_SCREENINGS, NULL, (struct cms_response**)response, result_bitmap);
}

extern int cms_delete_screening(cms_t cms, struct cms_delete_screening_request request, struct cms_delete_screening_response** response) {
	struct cms_request_param request_params[] = {
			CMS_REQUEST_PARAM_INFO(request.cinema_id),
			CMS_REQUEST_PARAM_INFO(request.hall_number),
			CMS_REQUEST_PARAM_INFO(request.date),
			CMS_REQUEST_PARAM_INFO(request.start_time)
	};
	return cms_operation_execute(cms, DELETE_SCREENING, request_params, (struct cms_response**)response, NULL);
}
