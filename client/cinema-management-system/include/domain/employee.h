#pragma once

#include <stdint.h>

enum role { PROJECTIONIST, USHER };

typedef struct employee* employee_t;

extern int32_t get_id(employee_t employee);

extern char* get_name(employee_t employee);

extern char* get_surname(employee_t employee);

extern enum role get_role(employee_t employee);
