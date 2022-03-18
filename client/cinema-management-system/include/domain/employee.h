#pragma once

#include <stdint.h>

enum role {
	PROJECTIONIST,
	USHER
};

typedef struct employee* employee_t {
	int32_t id;
	char* name;
	char* surname;
	enum role role;
};
