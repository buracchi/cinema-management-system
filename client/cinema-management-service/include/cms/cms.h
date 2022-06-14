#pragma once

#include <stdint.h>
#include <stdbool.h>

#define CMS_DECIMAL_LEN 18
#define CMS_DATE_LEN 11	// "YYYY-MM-DD" format
#define CMS_TIME_LEN 9	// "hh:mm:ss" format
#define CMS_DAY_LEN 16
#define CMS_CINEMA_ADDRESS_LEN 128
#define CMS_FILM_NAME_LEN 45

typedef struct cms* cms_t;

struct cms_credentials {
	const char* username;
	const char* password;
	const char* host;
	const char* db;
	unsigned int port;
};

struct cms_response {
	const char* error_message;
	uint64_t num_elements;
};

extern cms_t cms_init(struct cms_credentials* credentials);

extern void cms_destroy(cms_t cms);

extern const char* cms_get_error_message(cms_t cms);

extern void cms_destroy_response(struct cms_response* response);
