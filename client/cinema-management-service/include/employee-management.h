#pragma once

#include <stdint.h>
#include <stdbool.h>

#include "cms.h"
#include "domain/employee.h"

struct cms_get_employees_response { struct employee_t* employee; };
extern bool cms_get_employees(cms_t cms, struct cms_get_employees_response* response);

struct cms_add_employee_request { const char* name; const char* surname; enum role role; };
extern bool cms_add_employee(cms_t cms, struct cms_add_employee_request request);

struct cms_delete_employee_request { int32_t employee_id; };
extern bool cms_delete_employee(cms_t cms, struct cms_delete_employee_request request);
