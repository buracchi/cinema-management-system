#pragma once
#ifndef __unix__
#pragma warning(disable:4200)
#pragma warning(disable:4820)
#endif

#include <cms/cms.h>

#include <stdint.h>
#include <stdbool.h>

struct cms_get_all_cinema_response {
	struct cms_response;
	struct cms_cinema {
		int32_t id;
		char address[CMS_CINEMA_ADDRESS_LEN];
		char opening_time[CMS_TIME_LEN];
		char closing_time[CMS_TIME_LEN];
	} result[]; };
extern int cms_get_all_cinema(cms_t cms, struct cms_get_all_cinema_response** response);

struct cms_get_cinema_screenings_request { int32_t cinema_id; };
struct cms_get_cinema_screenings_response{
	struct cms_response;
	struct cms_screening {
		int32_t cinema_id;
		int32_t hall_id;
		char date[CMS_DATE_LEN];
		char time[CMS_TIME_LEN];
		char price[CMS_DECIMAL_LEN];
		char film_name[CMS_FILM_NAME_LEN];
		char running_time[CMS_TIME_LEN];
		char film_studio[256];
		char cast[1024];
	} result[]; };
extern int cms_get_cinema_screenings(cms_t cms, struct cms_get_cinema_screenings_request request, struct cms_get_cinema_screenings_response** response);

struct cms_get_cinema_halls_request { int32_t cinema_id; };
struct cms_get_cinema_halls_response { struct cms_response; struct cms_hall_info { int32_t id; uint64_t num_rows; uint64_t num_cols; } result[]; };
extern int cms_get_cinema_halls(cms_t cms, struct cms_get_cinema_halls_request request, struct cms_get_cinema_halls_response** response);

struct cms_get_available_seats_request {int32_t cinema_id; int32_t hall_id; const char date[CMS_DATE_LEN]; const char start_time[CMS_TIME_LEN]; };
struct cms_get_available_seats_response {struct cms_response; struct cms_seat { char row; uint32_t number; } result[]; };
extern int cms_get_available_seats(cms_t cms, struct cms_get_available_seats_request request, struct cms_get_available_seats_response** response);

struct cms_book_seat_request {
	int32_t cinema_id;
	int32_t hall_id;
	const char date[CMS_DATE_LEN];
	const char start_time[CMS_TIME_LEN];
	char seat_row;
	int32_t seat_number;
	const char name_on_card[128];
	const char card_number[17];
	const char expiry_date[CMS_DATE_LEN];
	const char security_code[4]; };
struct cms_book_seat_response { struct cms_response; struct cms_book_seat_result { int32_t booking_code; } result[]; };
extern int cms_book_seat(cms_t cms, struct cms_book_seat_request request, struct cms_book_seat_response** response);

struct cms_cancel_booking_request { int32_t booking_code; };
struct cms_cancel_booking_response { struct cms_response; };
extern int cms_cancel_booking(cms_t cms, struct cms_cancel_booking_request request, struct cms_cancel_booking_response** response);

struct cms_validate_booking_request { int32_t booking_code; };
struct cms_validate_booking_response { struct cms_response; };
extern int cms_validate_booking(cms_t cms, struct cms_validate_booking_request request, struct cms_validate_booking_response** response);
