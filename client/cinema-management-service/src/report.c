#include <cms/report.h>

#include "cms_operation.h"

extern struct cms_response cms_get_screenings_without_projectionist(cms_t cms, CMS_OUT struct cms_screening_without_projectionist** screenings_without_projectionist) {
	struct cms_result_metadata result_metadata = {
		.ptr = (void**)screenings_without_projectionist,
		.size = sizeof(struct cms_screening_without_projectionist),
		.offset_size_bitmap = (struct cms_result_bitmap[]) {
			CMS_RESULT_BITMAP_INFO(struct cms_screening_without_projectionist, cinema, hall, date, time, price, film_name)
		}
	};
	return cms_operation_execute(cms, GET_SCREENINGS_WITHOUT_PROJECTIONIST, NULL, &result_metadata);
}

extern struct cms_response cms_get_cinema_without_ushers(cms_t cms, CMS_OUT struct cms_cinema_without_ushers** cinema_without_ushers) {
	struct cms_result_metadata result_metadata = {
		.ptr = (void**)cinema_without_ushers,
		.size = sizeof(struct cms_cinema_without_ushers),
		.offset_size_bitmap = (struct cms_result_bitmap[]) {
			CMS_RESULT_BITMAP_INFO(struct cms_cinema_without_ushers, cinema, address, day, since, until, ushers)
		}
	};
	return cms_operation_execute(cms, GET_CINEMA_WITHOUT_ENOUGH_USHERS, NULL, &result_metadata);
}

extern struct cms_response cms_get_reservations_status(cms_t cms, CMS_OUT struct cms_reservations_status** reservations_status) {
	struct cms_result_metadata result_metadata = {
		.ptr = (void**)reservations_status,
		.size = sizeof(struct cms_reservations_status),
		.offset_size_bitmap = (struct cms_result_bitmap[]) {
			CMS_RESULT_BITMAP_INFO(struct cms_reservations_status, cinema, address, hall, total, confirmed, cancelled, validated, expired)
		}
	};
	return cms_operation_execute(cms, GET_MONTHLY_RESERVATIONS_STATUS, NULL, &result_metadata);
}
