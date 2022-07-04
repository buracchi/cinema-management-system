#include <cms/employee_management.h>

#include "cms_operation.h"

extern struct cms_response cms_get_employees(cms_t cms, CMS_OUT struct cms_employee** employees) {
	struct cms_result_metadata result_metadata = {
		.ptr = (void**)employees,
		.size = sizeof(struct cms_employee),
		.offset_size_bitmap = (struct cms_result_bitmap[]) {
			CMS_RESULT_BITMAP_INFO(struct cms_employee, id, name, surname, role)
		}
	};
	return cms_operation_execute(cms, GET_EMPLOYEES, NULL, &result_metadata);
}

extern struct cms_response cms_add_employee(cms_t cms, CMS_IN const struct cms_employee_details* details) {
	struct cms_request_param request_params[] = {
			CMS_REQUEST_PARAM_INFO(details->name),
			CMS_REQUEST_PARAM_INFO(details->surname),
			CMS_REQUEST_PARAM_INFO(details->role)
	};
	return cms_operation_execute(cms, ADD_EMPLOYEE, &request_params, NULL);
}

extern struct cms_response cms_delete_employee(cms_t cms, CMS_IN int32_t id) {
	struct cms_request_param request_params[] = {
			CMS_REQUEST_PARAM_INFO(id)
	};
	return cms_operation_execute(cms, DELETE_EMPLOYEE, &request_params, NULL);
}
