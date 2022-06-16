#pragma once

#include <cms/cms.h>

#include <stdint.h>
#include <stdbool.h>

struct cms_get_screenings_response {
	struct cms_response;
	struct cms_screening {
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

struct cms_get_movies_response {
	struct cms_response;
	struct cms_movie {
		int32_t film_id;
		char name[CMS_FILM_NAME_LEN];
		char duration[CMS_TIME_LEN];
		char studio[CMS_FILM_STUDIO_LEN];
		char cast[CMS_FILM_CAST_LEN];
	} result[]; };
extern int cms_get_movies(cms_t cms, struct cms_get_movies_response** response);

struct cms_add_screening_request {
	int32_t cinema_id;
	int32_t hall_number;
	char date[CMS_DATE_LEN];
	char star_time[CMS_TIME_LEN];
	char price[CMS_DECIMAL_LEN];
	int32_t film_id;
};
struct cms_add_screening_response {struct cms_response; };
extern int cms_add_screening(cms_t cms, struct cms_add_screening_request request, struct cms_add_screening_response** response);

struct cms_delete_screening_request { int32_t cinema_id; int32_t hall_number; char date[CMS_DATE_LEN]; char start_time[CMS_TIME_LEN]; };
struct cms_delete_screening_response { struct cms_response; };
extern int cms_delete_screening(cms_t cms, struct cms_delete_screening_request request, struct cms_delete_screening_response** response);

struct cms_get_available_projectionists_request { int32_t cinema_id; int32_t hall_number; char date[CMS_DATE_LEN]; char start_time[CMS_TIME_LEN]; };
struct cms_get_available_projectionists_response{
	struct cms_response;
	struct cms_available_projectionist {
		int32_t id;
		char name[CMS_EMPLOYEE_NAME_LEN];
		char surname[CMS_EMPLOYEE_SURNAME_LEN];
	} result[]; };
extern int cms_get_available_projectionists(cms_t cms, struct cms_get_available_projectionists_request request, struct cms_get_available_projectionists_response** response);

struct cms_assign_projectionist_request { int32_t projectionist_id; int32_t cinema_id; int32_t hall_number; char date[CMS_DATE_LEN]; char start_time[CMS_TIME_LEN]; };
struct cms_assign_projectionist_response{ struct cms_response; };
extern int cms_assign_projectionist(cms_t cms, struct cms_assign_projectionist_request request, struct cms_assign_projectionist_response** response);
