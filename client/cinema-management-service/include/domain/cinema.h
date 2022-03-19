#pragma once

#include <stdint.h>

typedef struct cinema* cinema_t;

extern int32_t cinema_get_id(cinema_t cinema);

extern char* cinema_get_address(cinema_t cinema);

extern char* cinema_get_opening_time(cinema_t cinema);

extern char* cinema_get_closing_time(cinema_t cinema);
