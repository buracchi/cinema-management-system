#pragma once

#include <stdint.h>
#include <stdbool.h>

#include "cms.h"

struct cms_get_all_cinema_response {
	struct cms_result_response;
	struct cms_cinema {
		int32_t id;
		char address[128];
		char opening_time[TIME_LEN];	// "hh:mm:ss" format
		char closing_time[TIME_LEN];	// "hh:mm:ss" format
	} result[]; };
extern bool cms_get_all_cinema(cms_t cms, struct cms_get_all_cinema_response** response);

struct cms_get_cinema_screenings_request { int32_t cinema_id; };
struct cms_get_cinema_screenings_response{
	struct cms_result_response;
	struct cms_screening {
		int32_t cinema_id;
		int32_t hall_id;
		char date[DATE_LEN];			//'YYYY-MM-DD' format
		char time[TIME_LEN];			//'hh:mm:ss' format
		char price[18];
		char film_name[45];
		char running_time[TIME_LEN];	// 'hh:mm:ss' format
		char film_studio[256];
		char cast[1024];
	} result[]; };
extern bool cms_get_cinema_screenings(cms_t cms, struct cms_get_cinema_screenings_request request, struct cms_get_cinema_screenings_response** response);

struct cms_get_cinema_halls_request { int32_t cinema_id; };
struct cms_get_cinema_halls_response { struct cms_result_response; struct cms_hall_info { int32_t id; uint64_t num_rows; uint64_t num_cols; } result[]; };
extern bool cms_get_cinema_halls(cms_t cms, struct cms_get_cinema_halls_request request, struct cms_get_cinema_halls_response** response);

struct cms_get_available_seats_request {int32_t cinema_id; int32_t hall_id; const char date[DATE_LEN]; const char start_time[TIME_LEN]; };
struct cms_get_available_seats_response {struct cms_result_response;
struct cms_seat { char row; uint32_t number; } result[]; };
extern bool cms_get_available_seats(cms_t cms, struct cms_get_available_seats_request request, struct cms_get_available_seats_response** response);

struct cms_book_seat_request {
	int32_t cinema_id;
	int32_t hall_id;
	const char date[11];
	const char start_time[9];
	char seat_row;
	int32_t seat_number;
	const char name_on_card[128];
	const char card_number[17];
	const char expiry_date[DATE_LEN];
	const char security_code[4]; };
struct cms_book_seat_response { struct cms_result_response; struct cms_book_seat_result { int32_t booking_code; } result[]; };
extern bool cms_book_seat(cms_t cms, struct cms_book_seat_request request, struct cms_book_seat_response** response);

struct cms_cancel_booking_request { int32_t booking_code; };
struct cms_cancel_booking_response { struct cms_result_response; };
extern bool cms_cancel_booking(cms_t cms, struct cms_cancel_booking_request request, struct cms_cancel_booking_response** response);

struct cms_validate_booking_request { int32_t booking_code; };
struct cms_validate_booking_response { struct cms_result_response; };
extern bool cms_validate_booking(cms_t cms, struct cms_validate_booking_request request, struct cms_validate_booking_response** response);
