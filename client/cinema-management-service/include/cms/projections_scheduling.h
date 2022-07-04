#pragma once

#include <cms/cms.h>

#include <stdint.h>
#include <stdbool.h>

struct cms_projection {
	int32_t cinema_id;
	int32_t hall_number;
	char date[CMS_DATE_LEN];
	char start_time[CMS_TIME_LEN];
};

struct cms_projection_details {
	char date[CMS_DATE_LEN];
	int32_t cinema_id;
	char cinema_address[CMS_CINEMA_ADDRESS_LEN];
	int32_t hall_number;
	int32_t film_id;
	char film_name[CMS_FILM_NAME_LEN];
	char price[CMS_DECIMAL_LEN];
	char start_time[CMS_TIME_LEN];
	char duration[CMS_TIME_LEN];
	int32_t projectionist_id;
	char projectionist_name[CMS_EMPLOYEE_NAME_LEN];
	char projectionist_surname[CMS_EMPLOYEE_SURNAME_LEN];
};

struct cms_add_projection_details {
	int32_t cinema_id;
	int32_t hall_number;
	char date[CMS_DATE_LEN];
	char start_time[CMS_TIME_LEN];
	char price[CMS_DECIMAL_LEN];
	int32_t film_id;
};

struct cms_movie {
	int32_t film_id;
	char name[CMS_FILM_NAME_LEN];
	char duration[CMS_TIME_LEN];
	char studio[CMS_FILM_STUDIO_LEN];
	char cast[CMS_FILM_CAST_LEN];
};

struct cms_available_projectionist {
	int32_t id;
	char name[CMS_EMPLOYEE_NAME_LEN];
	char surname[CMS_EMPLOYEE_SURNAME_LEN];
};

struct cms_assign_projectionist_details {
	int32_t projectionist_id;
	int32_t cinema_id;
	int32_t hall_number;
	char date[CMS_DATE_LEN];
	char start_time[CMS_TIME_LEN];
};

extern struct cms_response cms_get_projections(
	cms_t cms,
	CMS_OUT struct cms_projection_details** projections
);

extern struct cms_response cms_get_movies(
	cms_t cms,
	CMS_OUT struct cms_movie** movies
);

extern struct cms_response cms_add_projection(
	cms_t cms,
	CMS_IN const struct cms_add_projection_details* details
);

extern struct cms_response cms_delete_projection(
	cms_t cms,
	CMS_IN const struct cms_projection* projection
);

extern struct cms_response cms_get_available_projectionists(
	cms_t cms,
	CMS_IN const struct cms_projection* projection,
	CMS_OUT struct cms_available_projectionist** projectionists
);

extern struct cms_response cms_assign_projectionist(
	cms_t cms,
	CMS_IN const struct cms_assign_projectionist_details* details
);
