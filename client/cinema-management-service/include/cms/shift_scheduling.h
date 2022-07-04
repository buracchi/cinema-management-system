#pragma once

#include <cms/cms.h>

#include <stdint.h>
#include <stdbool.h>

struct cms_shift {
	int32_t employee_id;
	char day[CMS_DAY_LEN];
	char start_time[CMS_TIME_LEN];
};

struct cms_shift_details {
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

struct cms_add_shift_details {
	int32_t employee_id;
	char day[CMS_DAY_LEN];
	char start_time[CMS_TIME_LEN];
	char duration[CMS_TIME_LEN];
	int32_t cinema_id;
};

extern struct cms_response cms_get_shifts(
	cms_t cms,
	CMS_OUT struct cms_shift_details** shifts
);

extern struct cms_response cms_add_shift(
	cms_t cms, 
	CMS_IN const struct cms_add_shift_details* shift_details
);

extern struct cms_response cms_delete_shift(
	cms_t cms, 
	CMS_IN const struct cms_shift* shift
);
