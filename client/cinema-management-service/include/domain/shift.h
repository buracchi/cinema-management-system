#pragma once

#include <stdint.h>

#include "cinema.h"
#include "employee.h"
#include "weekday.h"

typedef struct shift* shift_t;

extern employee_t shift_get_employee(shift_t shift);

extern enum weekday shift_get_weekday(shift_t shift);

extern char* shift_get_opening_time(shift_t shift);

extern char* shift_get_closing_time(shift_t shift);

extern cinema_t shift_get_cinema(shift_t shift);
