#pragma once

#include <cms/cms.h>

#include <stdint.h>
#include <stdbool.h>

struct cms_screening_without_projectionist {
	int32_t cinema;
	int32_t hall;
	char date[CMS_DATE_LEN];
	char time[CMS_TIME_LEN];
	char price[CMS_DECIMAL_LEN];
	char film_name[CMS_FILM_NAME_LEN];
};

struct cms_cinema_without_ushers {
	int32_t cinema;
	char address[CMS_CINEMA_ADDRESS_LEN];
	char day[CMS_DAY_LEN];
	char since[CMS_TIME_LEN];
	char until[CMS_TIME_LEN];
	char ushers[CMS_DECIMAL_LEN];
};

struct cms_reservations_status {
	int32_t cinema;
	char address[CMS_CINEMA_ADDRESS_LEN];
	int32_t hall;
	char total[CMS_DECIMAL_LEN];
	char confirmed[CMS_DECIMAL_LEN];
	char cancelled[CMS_DECIMAL_LEN];
	char validated[CMS_DECIMAL_LEN];
	char expired[CMS_DECIMAL_LEN];
};

extern struct cms_response cms_get_screenings_without_projectionist(
	cms_t cms, 
	CMS_OUT struct cms_screening_without_projectionist** screenings_without_projectionist
);

extern struct cms_response cms_get_cinema_without_ushers(
	cms_t cms, 
	CMS_OUT struct cms_cinema_without_ushers** cinema_without_ushers
);

extern struct cms_response cms_get_reservations_status(
	cms_t cms, 
	CMS_OUT struct cms_reservations_status** reservations_status
);
