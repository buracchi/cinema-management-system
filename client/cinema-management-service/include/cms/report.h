#pragma once

#include <cms/cms.h>

#include <stdint.h>
#include <stdbool.h>

struct cms_screenings_without_projectionist {
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
	char confirmed[CMS_DECIMAL_LEN];
	char cancelled[CMS_DECIMAL_LEN];
	char validated[CMS_DECIMAL_LEN];
	char expired[CMS_DECIMAL_LEN];
};

struct cms_get_screenings_without_projectionist_response {
	struct cms_response;
	struct cms_screenings_without_projectionist result[];
};
extern int cms_get_screenings_without_projectionist(cms_t cms, struct cms_get_screenings_without_projectionist_response** response);

struct cms_get_cinema_without_ushers_response {
	struct cms_response;
	struct cms_cinema_without_ushers result[];
};
extern int cms_get_cinema_without_ushers(cms_t cms, struct cms_get_cinema_without_ushers_response** response);

struct cms_get_reservations_status_response {
	struct cms_response;
	struct cms_reservations_status result[];
};
extern int cms_get_reservations_status(cms_t cms, struct cms_get_reservations_status_response** response);
