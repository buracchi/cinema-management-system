#pragma once

#include <stdint.h>

#include "cinema.h"
#include "employee.h"
#include "weekday.h"

typedef struct shift* shift_t {
	employee_t employee;
	enum weekday weekday;
	char[9] opening_time;	// "hh:mm:ss" format
	char[9] closing_time;	// "hh:mm:ss" format
	cinema_t cinema;
};
