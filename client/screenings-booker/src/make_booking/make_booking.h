#pragma once

#include <cms/cms.h>

struct booking_data {
	int32_t cinema_id;
	char cinema_address[128];
	int32_t hall;
	char date[CMS_DATE_LEN];
	char time[CMS_TIME_LEN];
	char film_name[45];
	char price[18];
	char seat_row;
	uint32_t seat_number;
	int32_t booking_code;
};

extern int choose_cinema(cms_t cms, struct booking_data* booking_data);
extern int choose_screening(cms_t cms, struct booking_data* booking_data);
extern int choose_seat(cms_t cms, struct booking_data* booking_data);
extern int make_payment(cms_t cms, struct booking_data* booking_data);
