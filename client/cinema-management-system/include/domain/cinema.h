#pragma once

#include <stdint.h>

typedef struct cinema* cinema_t {
	int32_t id;
	char* address;
	char opening_time[9];	// "hh:mm:ss" format
	char closing_time[9];	// "hh:mm:ss" format
};
