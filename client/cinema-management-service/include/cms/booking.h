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

extern struct cms_response cms_get_all_cinema(
	cms_t cms,
	CMS_OUT struct cms_cinema** cinema
);

extern struct cms_response cms_get_cinema_screenings(
	cms_t cms,
	CMS_IN int32_t cinema_id,
	CMS_OUT struct cms_screening_details** screening_details
);

extern struct cms_response cms_get_cinema_halls(
	cms_t cms,
	CMS_IN int32_t cinema_id,
	CMS_OUT struct cms_hall** halls
);

extern struct cms_response cms_get_available_seats(
	cms_t cms,
	CMS_IN const struct cms_screening* screening,
	CMS_OUT struct cms_seat** seats
);

extern struct cms_response cms_create_booking(
	cms_t cms,
	CMS_IN const struct cms_booking_details* details,
	CMS_OUT char(**booking_code)[CMS_BOOKING_CODE_LEN]
);

extern struct cms_response cms_commit_booking(
	cms_t cms,
	CMS_IN const struct cms_payment_details* payment_details
);

extern struct cms_response cms_abort_booking(
	cms_t cms,
	CMS_IN const char(*booking_code)[CMS_BOOKING_CODE_LEN]
);

extern struct cms_response cms_cancel_booking(
	cms_t cms,
	CMS_IN const char(*booking_code)[CMS_BOOKING_CODE_LEN]
);

extern struct cms_response cms_validate_booking(
	cms_t cms,
	CMS_IN const char(*booking_code)[CMS_BOOKING_CODE_LEN]
);
