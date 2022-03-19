#pragma once

#include <stdint.h>

#include "hall.h"

typedef struct seat* seat_t;

extern char seat_get_row(seat_t seat);

extern uint8_t seat_get_number(seat_t seat);

extern hall_t seat_get_hall(seat_t seat);
