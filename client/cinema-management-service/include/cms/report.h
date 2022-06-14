#pragma once

#include <cms/cms.h>

#include <stdint.h>
#include <stdbool.h>

struct cms_get_screenings_without_projectionist_response {
	struct cms_response;
	struct cms_screenings_without_projectionist {
		int32_t cinema;
		int32_t hall;
		char date[DATE_LEN];	// "YYYY-MM-DD" format
		char time[TIME_LEN];	// "hh:mm:ss" format
		char price[18];
		char film_name[45];
	} result[]; };
extern int cms_get_screenings_without_projectionist(cms_t cms, struct cms_get_screenings_without_projectionist_response** response);

struct cms_get_cinema_without_ushers_response {
	struct cms_response;
	struct cms_cinema_without_ushers {
		int32_t cinema;
		char address[128];
		char day[16];
		char since[TIME_LEN];	// "hh:mm:ss" format
		char until[TIME_LEN];	// "hh:mm:ss" format
		char ushers[18];
	} result[]; };
extern int cms_get_cinema_without_ushers(cms_t cms, struct cms_get_cinema_without_ushers_response** response);

struct cms_get_reservations_status_response {
	struct cms_response;
	struct cms_reservations_status {
		int32_t cinema;
		char address[128];
		int32_t hall;
		char confirmed[18];
		char cancelled[18];
		char validated[18];
		char expired[18];
	} result[]; };
extern int cms_get_reservations_status(cms_t cms, struct cms_get_reservations_status_response** response);
