#pragma once

#include <cms/cms.h>

#include <stdint.h>
#include <stdbool.h>

struct cms_cinema {
	int32_t id;
	char address[CMS_CINEMA_ADDRESS_LEN];
	char opening_time[CMS_TIME_LEN];
	char closing_time[CMS_TIME_LEN];
};

struct cms_hall {
	int32_t id;
	uint64_t num_rows;
	uint64_t num_cols;
};

struct cms_seat {
	char row[CMS_HALL_ROW_LEN];
	uint32_t number;
};

struct cms_screening {
	int32_t cinema_id;
	int32_t hall_number;
	char date[CMS_DATE_LEN];
	char start_time[CMS_TIME_LEN];
};

struct cms_screening_details {
	int32_t cinema_id;
	int32_t hall_number;
	char date[CMS_DATE_LEN];
	char start_time[CMS_TIME_LEN];
	char price[CMS_DECIMAL_LEN];
	char film_name[CMS_FILM_NAME_LEN];
	char running_time[CMS_TIME_LEN];
	char film_studio[CMS_FILM_STUDIO_LEN];
	char cast[CMS_FILM_CAST_LEN];
};

struct cms_booking {
	char booking_code[CMS_BOOKING_CODE_LEN];
};

struct cms_booking_details {
	int32_t cinema_id;
	int32_t hall_number;
	char date[CMS_DATE_LEN];
	char start_time[CMS_TIME_LEN];
	char seat_row[CMS_HALL_ROW_LEN];
	int32_t seat_number;
};

struct cms_payment_details {
	char booking_code[CMS_BOOKING_CODE_LEN];
	char name_on_card[128];
	char card_number[17];
	char expiry_date[CMS_DATE_LEN];
	char security_code[4];
};

struct cms_get_all_cinema_response {
	struct cms_response;
	struct cms_cinema result[];
};
extern int cms_get_all_cinema(cms_t cms, struct cms_get_all_cinema_response** response);

struct cms_get_cinema_screenings_response {
	struct cms_response;
	struct cms_screening_details result[];
};
extern int cms_get_cinema_screenings(cms_t cms, int32_t cinema_id, struct cms_get_cinema_screenings_response** response);

struct cms_get_cinema_halls_response {
	struct cms_response;
	struct cms_hall result[];
};
extern int cms_get_cinema_halls(cms_t cms, int32_t cinema_id, struct cms_get_cinema_halls_response** response);

struct cms_get_available_seats_response {
	struct cms_response;
	struct cms_seat result[];
};
extern int cms_get_available_seats(cms_t cms, struct cms_screening* screening, struct cms_get_available_seats_response** response);

struct cms_create_booking_response {
	struct cms_response;
	struct cms_booking result[];
};
extern int cms_create_booking(cms_t cms, struct cms_booking_details* details, struct cms_create_booking_response** response);

struct cms_commit_booking_response {
	struct cms_response;
};
extern int cms_commit_booking(cms_t cms, struct cms_payment_details* payment_details, struct cms_commit_booking_response** response);

struct cms_abort_booking_response {
	struct cms_response;
};
extern int cms_abort_booking(cms_t cms, char (*booking_code)[CMS_BOOKING_CODE_LEN], struct cms_abort_booking_response** response);

struct cms_cancel_booking_response {
	struct cms_response;
};
extern int cms_cancel_booking(cms_t cms, char (*booking_code)[CMS_BOOKING_CODE_LEN], struct cms_cancel_booking_response** response);

struct cms_validate_booking_response {
	struct cms_response;
};
extern int cms_validate_booking(cms_t cms, char (*booking_code)[CMS_BOOKING_CODE_LEN], struct cms_validate_booking_response** response);
