#pragma once

#include <stdint.h>

#include "cinema.h"

typedef struct hall* hall_t;

extern uint8_t get_number(hall_t hall);

extern cinema_t get_cinema(hall_t hall);
