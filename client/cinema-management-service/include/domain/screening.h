#pragma once

#include <stdint.h>

#include "hall.h"
#include "employee.h"
#include "film.h"

typedef struct screening* screening_t;

extern hall_t screening_get_hall(screening_t screening);

extern char* screening_get_date(screening_t screening);

extern char* screening_get_time(screening_t screening);

extern uint64_t screening_get_price_decimal(screening_t screening);

extern uint8_t screening_get_price_cents(screening_t screening);

extern film_t screening_get_film(screening_t screening);

extern employee_t screening_get_projectionist(screening_t screening);
