#pragma once

#include <cms/cms.h>

struct booking_data {
	int32_t cinema_id;
	char cinema_address[128];
	uint8_t hall;
	char date[DATE_LEN];
	char time[TIME_LEN];
	char film_name[45];
	char price[18];
	char seat_row;
	uint8_t seat_number;
	int32_t booking_code;
};

extern int choose_cinema(cms_t cms, struct booking_data* booking_data);
extern int choose_screening(cms_t cms, struct booking_data* booking_data);
extern int choose_seat(cms_t cms, struct booking_data* booking_data);
extern int make_payment(cms_t cms, struct booking_data* booking_data);
