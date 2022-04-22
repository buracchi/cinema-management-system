#pragma once

#include <stdint.h>
#include <stdbool.h>

#define DATE_LEN 11
#define TIME_LEN 9

typedef struct cms* cms_t;

struct cms_response {
	const char* error_message;
};

struct cms_result_response {
	struct cms_response;
	uint64_t num_elements;
};

extern cms_t cms_init(const char* username, const char* password);

extern bool cms_destroy(cms_t cms);

extern const char* cms_get_error_message(cms_t cms);

extern void cms_destroy_response(struct cms_response* response);
