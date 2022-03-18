#pragma once

#include <stdint.h>

#include "hall.h"
#include "employee.h"
#include "film.h"

typedef struct screening* screening_t;

extern hall_t get_hall(screening_t screening);

extern char get_date(screening_t screening);

extern char get_time(screening_t screening);

extern uint64_t get_price_decimal(screening_t screening);

extern uint8_t get_price_cents(screening_t screening);

extern film_t get_film(screening_t screening);

extern employee_t get_projectionist(screening_t screening);
