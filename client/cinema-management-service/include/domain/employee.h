#pragma once

#include <stdint.h>

enum role { PROJECTIONIST, USHER };

typedef struct employee* employee_t;

extern int32_t employee_get_id(employee_t employee);

extern char* employee_get_name(employee_t employee);

extern char* employee_get_surname(employee_t employee);

extern enum role employee_get_role(employee_t employee);
