#pragma once

#include <stdint.h>

#include "hall.h"
#include "employee.h"
#include "film.h"

typedef struct screening* screening_t{
	hall_t hall;
	char date[11];	//'YYYY-MM-DD' format
	char time[9];	//'hh:mm:ss' format
	uint64_t price_decimal;
	uint8_t price_cents;
	film_t film;
	employee_t projectionist;
};
