#pragma once

#include <stdint.h>

typedef struct cinema* cinema_t;

extern int32_t get_id(cinema_t cinema);

extern char* get_address(cinema_t cinema);

extern char* get_opening_time(cinema_t cinema);

extern char* get_closing_time(cinema_t cinema);
