#pragma once

#include <stdint.h>

#include "cinema.h"

typedef struct hall* hall_t;

extern uint8_t hall_get_number(hall_t hall);

extern cinema_t hall_get_cinema(hall_t hall);
