#pragma once

#include <cms/cms.h>

struct booking_data {
	int32_t cinema_id;
	char cinema_address[CMS_CINEMA_ADDRESS_LEN];
	int32_t hall;
	char date[CMS_DATE_LEN];
	char time[CMS_TIME_LEN];
	char film_name[CMS_FILM_NAME_LEN];
	char price[CMS_DECIMAL_LEN];
	char seat_row[CMS_HALL_ROW_LEN];
	uint32_t seat_number;
	char booking_code[CMS_BOOKING_CODE_LEN];
};

extern int choose_cinema(cms_t cms, struct booking_data* booking_data);
extern int choose_screening(cms_t cms, struct booking_data* booking_data);
extern int choose_seat(cms_t cms, struct booking_data* booking_data);
extern int make_payment(cms_t cms, struct booking_data* booking_data);
