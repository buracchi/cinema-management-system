#pragma once

#include <stdint.h>
#include <errno.h>

#include "cinema-management-service.h"
#include "domain/employee.h"

struct get_employees_response { struct employee_t* employee; };
extern errno_t get_employees(cinema_management_service_t service, struct get_employees_response* response);

struct add_employee_request { const char* name; const char* surname; enum role role; };
extern errno_t add_employee(cinema_management_service_t service, struct add_employee_request request);

struct delete_employee_request { int32_t employee_id; };
extern errno_t delete_employee(cinema_management_service_t service, struct delete_employee_request request);
