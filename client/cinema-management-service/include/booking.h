#pragma once

#include <stdint.h>
#include <errno.h>

#include "cinema-management-service.h"
#include "domain/cinema.h"
#include "domain/employee.h"
#include "domain/screening.h"
#include "domain/seat.h"

struct get_all_cinema_response {
	struct result_response;
	struct cinema {
		int32_t id;
		char address[128];
		char opening_time[TIME_LEN];	// "hh:mm:ss" format
		char closing_time[TIME_LEN];	// "hh:mm:ss" format
	} result[]; };
extern errno_t get_all_cinema(cinema_management_service_t service, struct get_all_cinema_response** response);

struct get_cinema_screenings_request { int32_t cinema_id; };
struct get_cinema_screenings_response{
	struct result_response;
	struct screening {
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
extern errno_t get_cinema_screenings(cinema_management_service_t service, struct get_cinema_screenings_request request, struct get_cinema_screenings_response** response);

struct get_available_seats_request {
	int32_t cinema_id;
	int32_t hall_id;
	const char date[DATE_LEN];
	const char start_time[TIME_LEN]; };
struct get_available_seats_response {
	struct result_response; 
	struct hall_info {
		int32_t num_rows;
		int32_t num_cols;
	} hall_info_result;
	struct seat {
		char row;
		uint8_t number;
	} seat_result[];
};
extern errno_t get_available_seats(cinema_management_service_t service, struct get_available_seats_request request, struct get_available_seats_response** response);

struct book_seat_request {
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
struct book_seat_response { struct result_response; struct book_seat_result { int32_t booking_code; } result[]; };
extern errno_t book_seat(cinema_management_service_t service, struct book_seat_request request, struct book_seat_response** response);

struct cancel_booking_request { int32_t booking_code; };
extern errno_t cancel_booking(cinema_management_service_t service, struct cancel_booking_request request);

struct validate_booking_request { int32_t booking_code; };
struct validate_booking_response { struct response; };
extern errno_t validate_booking(cinema_management_service_t service, struct validate_booking_request request, struct validate_booking_response** response);
