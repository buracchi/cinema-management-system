#pragma once

#include <stdint.h>
#include <stdbool.h>

// MySQL doesn't NULL terminate strings, add +1 to the
// string type based columns length (CHAR, VARCHAR, ecc...)
#define CMS_DECIMAL_LEN 18
#define CMS_DATE_LEN 11	// "YYYY-MM-DD" format
#define CMS_TIME_LEN 9	// "hh:mm:ss" format
#define CMS_BOOKING_CODE_LEN 7
#define CMS_CINEMA_ADDRESS_LEN 128
#define CMS_EMPLOYEE_NAME_LEN 64
#define CMS_EMPLOYEE_SURNAME_LEN 64
#define CMS_EMPLOYEE_ROLE_LEN 16
#define CMS_DAY_LEN 16
#define CMS_FILM_NAME_LEN 64
#define CMS_FILM_STUDIO_LEN 256
#define CMS_FILM_CAST_LEN 1024
#define CMS_HALL_ROW_LEN 2

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
