#pragma once

#include <stdint.h>

typedef struct film* film_t {
	uint32_t id;
	char* name;
	char running_time[9];	// 'hh:mm:ss' format
	char* film_studio;
	char* cast;
};
