#pragma once

#include <cms/cms.h>

#include <stdint.h>
#include <stdbool.h>

struct cms_cinema {
	int32_t id;
	char address[CMS_CINEMA_ADDRESS_LEN];
	char opening_time[CMS_TIME_LEN];
	char closing_time[CMS_TIME_LEN];
};

struct cms_hall_details {
	int32_t cinema_id;
	int32_t hall_number;
	int32_t rows;
	int32_t rows_seats;
};

struct cms_hall {
	int32_t id;
	uint64_t num_rows;
	uint64_t num_cols;
};

struct cms_cinema_details {
	char address[CMS_CINEMA_ADDRESS_LEN];
	char opening_time[CMS_TIME_LEN];
	char closing_time[CMS_TIME_LEN];
};

extern struct cms_response cms_get_all_cinema(
	cms_t cms,
	CMS_OUT struct cms_cinema** cinema
);

extern struct cms_response cms_add_cinema(
	cms_t cms,
	CMS_IN const struct cms_cinema_details* details
);

extern struct cms_response cms_delete_cinema(
	cms_t cms,
	CMS_IN int32_t cinema_id
);

extern struct cms_response cms_get_cinema_halls(
	cms_t cms,
	CMS_IN int32_t cinema_id,
	CMS_OUT struct cms_hall** halls
);

extern struct cms_response cms_add_hall(
	cms_t cms,
	CMS_IN const struct cms_hall_details* details
);

extern struct cms_response cms_delete_hall(
	cms_t cms,
	CMS_IN int32_t cinema_id,
	CMS_IN int32_t hall_number
);
