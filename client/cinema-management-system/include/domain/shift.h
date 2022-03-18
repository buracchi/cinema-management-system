#pragma once

#include <stdint.h>

#include "cinema.h"
#include "employee.h"
#include "weekday.h"

typedef struct shift* shift_t;

extern employee_t get_employee(shift_t shift);

extern enum weekday get_weekday(shift_t shift);

extern char* get_opening_time(shift_t shift);

extern char* get_closing_time(shift_t shift);

extern cinema_t get_cinema(shift_t shift);
