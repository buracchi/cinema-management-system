#pragma once

#include <cms/cms.h>

#include <stdint.h>
#include <stdbool.h>

struct cms_get_screenings_response {
	struct cms_response;
	struct cms_screenings {
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
	} result[]; };
extern int cms_get_screenings(cms_t cms, struct cms_get_screenings_response** response);

struct cms_delete_screening_request { int32_t cinema_id; int32_t hall_number; char date[CMS_DATE_LEN]; char start_time[CMS_TIME_LEN]; };
struct cms_delete_screening_response { struct cms_response; };
extern int cms_delete_screening(cms_t cms, struct cms_delete_screening_request request, struct cms_delete_screening_response** response);

/*
struct cms_add_screening_request { int32_t cinema_id; uint8_t hall_number; char day[CMS_DATE_LEN]; char start_time[CMS_TIME_LEN]; char price[18]; int32_t film_id; };
extern bool cms_add_screening(cms_t cms, struct cms_add_screening_request request);

struct cms_get_available_projectionists_request { int32_t employee_id; char day[CMS_DATE_LEN]; char start_time[CMS_TIME_LEN]; char duration[CMS_TIME_LEN]; int32_t cinema_id; };
struct cms_get_available_projectionists_response { employee_t* employees; };
extern bool cms_get_available_projectionists(cms_t cms, struct cms_get_available_projectionists_request request, struct cms_get_available_projectionists_response** response);

struct cms_add_screening_request { int32_t employee_id; int32_t cinema_id; uint8_t hall_number; char day[CMS_DATE_LEN]; char start_time[CMS_TIME_LEN]; };
extern bool cms_assign_projectionist(cms_t cms, struct cms_cms_add_screening_request request);

*/
