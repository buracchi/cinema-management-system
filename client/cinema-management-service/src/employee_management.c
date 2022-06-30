#include <cms/employee_management.h>

#include "cms_operation.h"

extern int cms_get_employees(cms_t cms, struct cms_get_employees_response** response) {
	struct cms_result_bitmap result_bitmap[] = {
			CMS_RESULT_BITMAP_INFO(struct cms_get_employees_response, result[0]),
			CMS_RESULT_BITMAP_INFO(struct cms_employee, id, name, surname, role)
	};
	return cms_operation_execute(cms, GET_EMPLOYEES, NULL, (struct cms_response**)response, result_bitmap);
}

extern int cms_add_employee(cms_t cms, struct cms_employee_details* details, struct cms_add_employee_response** response) {
	struct cms_request_param request_params[] = {
			CMS_REQUEST_PARAM_INFO(details->name),
			CMS_REQUEST_PARAM_INFO(details->surname),
			CMS_REQUEST_PARAM_INFO(details->role)
	};
	return cms_operation_execute(cms, ADD_EMPLOYEE, request_params, (struct cms_response**)response, NULL);
}

extern int cms_delete_employee(cms_t cms, int32_t id, struct cms_delete_employee_response** response) {
	struct cms_request_param request_params[] = {
			CMS_REQUEST_PARAM_INFO(id)
	};
	return cms_operation_execute(cms, DELETE_EMPLOYEE, request_params, (struct cms_response**)response, NULL);
}
