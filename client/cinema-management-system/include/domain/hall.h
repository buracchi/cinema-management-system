#pragma once

#include <stdint.h>

#include "cinema.h"

typedef struct hall* hall_t {
	uint8_t number;
	cinema_t cinema;
};
