#pragma once

#include <cms/cms.h>

#include <stdint.h>
#include <stdbool.h>

struct cms_get_employees_response {
	struct cms_response;
	struct cms_employee {
		int32_t id;
		char name[CMS_EMPLOYEE_NAME_LEN];
		char surname[CMS_EMPLOYEE_SURNAME_LEN];
		char role[CMS_EMPLOYEE_ROLE_LEN];
	} result[];
};
extern int cms_get_employees(cms_t cms, struct cms_get_employees_response** response);

struct cms_add_employee_request {
	const char name[CMS_EMPLOYEE_NAME_LEN];
	char surname[CMS_EMPLOYEE_SURNAME_LEN];
	char role[CMS_EMPLOYEE_ROLE_LEN];
};
struct cms_add_employee_response {
	struct cms_response;
};
extern int cms_add_employee(cms_t cms, struct cms_add_employee_request* request, struct cms_add_employee_response** response);

struct cms_delete_employee_request {
	int32_t id;
};
struct cms_delete_employee_response {
	struct cms_response;
};
extern int cms_delete_employee(cms_t cms, struct cms_delete_employee_request* request, struct cms_delete_employee_response** response);
