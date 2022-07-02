#include <cms/report.h>

#include "cms_operation.h"

extern int cms_get_screenings_without_projectionist(cms_t cms, struct cms_get_screenings_without_projectionist_response** response) {
	struct cms_result_bitmap result_bitmap[] = {
			CMS_RESULT_BITMAP_INFO(struct cms_get_screenings_without_projectionist_response, result[0]),
			CMS_RESULT_BITMAP_INFO(struct cms_screenings_without_projectionist, cinema, hall, date, time, price, film_name)
	};
	return cms_operation_execute(cms, GET_SCREENINGS_WITHOUT_PROJECTIONIST, NULL, (struct cms_response**)response, result_bitmap);
}

extern int cms_get_cinema_without_ushers(cms_t cms, struct cms_get_cinema_without_ushers_response** response) {
	struct cms_result_bitmap result_bitmap[] = {
			CMS_RESULT_BITMAP_INFO(struct cms_get_cinema_without_ushers_response, result[0]),
			CMS_RESULT_BITMAP_INFO(struct cms_cinema_without_ushers, cinema, address, day, since, until, ushers)
	};
	return cms_operation_execute(cms, GET_CINEMA_WITHOUT_ENOUGH_USHERS, NULL, (struct cms_response**)response, result_bitmap);
}

extern int cms_get_reservations_status(cms_t cms, struct cms_get_reservations_status_response** response) {
	struct cms_result_bitmap result_bitmap[] = {
			CMS_RESULT_BITMAP_INFO(struct cms_get_reservations_status_response, result[0]),
			CMS_RESULT_BITMAP_INFO(struct cms_reservations_status, cinema, address, hall, total, confirmed, cancelled, validated, expired)
	};
	return cms_operation_execute(cms, GET_MONTHLY_RESERVATIONS_STATUS, NULL, (struct cms_response**)response, result_bitmap);
}
