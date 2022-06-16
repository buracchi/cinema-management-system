#pragma once

#include <cms/cms.h>

#include <stdint.h>
#include <stdbool.h>

struct cms_get_all_cinema_response {
	struct cms_response;
	struct cms_cinema {
		int32_t id;
		char address[CMS_CINEMA_ADDRESS_LEN];
		char opening_time[CMS_TIME_LEN];
		char closing_time[CMS_TIME_LEN];
	} result[];
};
extern int cms_get_all_cinema(cms_t cms, struct cms_get_all_cinema_response** response);

struct cms_add_cinema_request {
	const char address[CMS_CINEMA_ADDRESS_LEN];
	char opening_time[CMS_TIME_LEN];
	char closing_time[CMS_TIME_LEN];
};
struct cms_add_cinema_response {
	struct cms_response;
};
extern int cms_add_cinema(cms_t cms, struct cms_add_cinema_request* request, struct cms_add_cinema_response** response);

struct cms_delete_cinema_request {
	int32_t id;
};
struct cms_delete_cinema_response {
	struct cms_response;
};
extern int cms_delete_cinema(cms_t cms, struct cms_delete_cinema_request* request, struct cms_delete_cinema_response** response);

struct cms_get_cinema_halls_request {
	int32_t cinema_id;
};
struct cms_get_cinema_halls_response {
	struct cms_response;
	struct cms_hall {
		int32_t id;
		uint64_t num_rows;
		uint64_t num_cols;
	} result[];
};
extern int cms_get_cinema_halls(cms_t cms, struct cms_get_cinema_halls_request* request, struct cms_get_cinema_halls_response** response);

struct cms_add_hall_request {
	int32_t cinema_id;
	int32_t hall_number;
	int32_t rows;
	int32_t rows_seats;
};
struct cms_add_hall_response {
	struct cms_response;
};
extern int cms_add_hall(cms_t cms, struct cms_add_hall_request* request, struct cms_add_hall_response** response);

struct cms_delete_hall_request {
	int32_t cinema_id;
	int32_t hall_number;
};
struct cms_delete_hall_response {
	struct cms_response;
};
extern int cms_delete_hall(cms_t cms, struct cms_delete_hall_request* request, struct cms_delete_hall_response** response);
