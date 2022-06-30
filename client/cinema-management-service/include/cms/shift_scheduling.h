#pragma once

#include <cms/cms.h>

#include <stdint.h>
#include <stdbool.h>

struct cms_shift {
	char day[CMS_DAY_LEN];
	char start_time[CMS_TIME_LEN];
	char duration[CMS_TIME_LEN];
	int32_t cinema_id;
	char cinema_address[CMS_CINEMA_ADDRESS_LEN];
	int32_t employee_id;
	char employee_name[CMS_EMPLOYEE_NAME_LEN];
	char employee_surname[CMS_EMPLOYEE_SURNAME_LEN];
	char employee_role[CMS_EMPLOYEE_ROLE_LEN];
};

struct cms_get_shifts_response {
	struct cms_response;
	struct cms_shift result[];
};
extern int cms_get_shifts(cms_t cms, struct cms_get_shifts_response** response);

struct cms_add_shift_request {
	int32_t employee_id;
	char day[CMS_DAY_LEN];
	char start_time[CMS_TIME_LEN];
	char duration[CMS_TIME_LEN];
	int32_t cinema_id;
};
struct cms_add_shift_response {
	struct cms_response;
};
extern int cms_add_shift(cms_t cms, struct cms_add_shift_request* request, struct cms_add_shift_response** response);

struct cms_delete_shift_request {
	int32_t employee_id;
	char day[CMS_DAY_LEN];
	char start_time[CMS_TIME_LEN];
};
struct cms_delete_shift_response {
	struct cms_response;
};
extern int cms_delete_shift(cms_t cms, struct cms_delete_shift_request* request, struct cms_delete_shift_response** response);
