#pragma once

#include <cms/cms.h>

#include <stdint.h>
#include <stdbool.h>

struct cms_employee {
	int32_t id;
	char name[CMS_EMPLOYEE_NAME_LEN];
	char surname[CMS_EMPLOYEE_SURNAME_LEN];
	char role[CMS_EMPLOYEE_ROLE_LEN];
};

struct cms_employee_details {
	char name[CMS_EMPLOYEE_NAME_LEN];
	char surname[CMS_EMPLOYEE_SURNAME_LEN];
	char role[CMS_EMPLOYEE_ROLE_LEN];
};

extern struct cms_response cms_get_employees(
	cms_t cms,
	CMS_OUT struct cms_employee** employees
);

extern struct cms_response cms_add_employee(
	cms_t cms,
	CMS_IN const struct cms_employee_details* details
);

extern struct cms_response cms_delete_employee(
	cms_t cms,
	CMS_IN int32_t id
);
