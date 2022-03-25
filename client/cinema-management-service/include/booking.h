#pragma once

#include <stdint.h>
#include <errno.h>

#include "cinema-management-service.h"
#include "domain/cinema.h"
#include "domain/employee.h"
#include "domain/screening.h"
#include "domain/seat.h"

struct get_all_cinema_response { cinema_t* cinema; };
extern errno_t get_all_cinema(cinema_management_service_t service, struct get_all_cinema_response* response);

struct get_cinema_screenings_request { int32_t cinema_id; };
struct get_cinema_screenings_response{ screening_t* screenings; };
extern errno_t get_cinema_screenings(cinema_management_service_t service, struct get_cinema_screenings_request request, struct get_cinema_screenings_response* response);

struct get_avalilable_seats_request { int32_t cinema_id; uint8_t hall_id; const char date[11]; const char start_time[9]; };
struct get_avalilable_seats_response { seat_t* available_seats; };
extern errno_t get_avalilable_seats(cinema_management_service_t service, struct get_avalilable_seats_request request, struct get_avalilable_seats_response* response);

struct book_seat_request {
	int32_t cinema_id;
	uint8_t hall_id;
	const char date[11];
	const char start_time[9];
	char seat_row;
	uint8_t seat_number;
	const char* name_on_card;
	uint64_t card_number;
	const char expiry_date[11];
	uint8_t security_code;};
struct book_seat_response { int32_t booking_code; };
extern errno_t book_seat(cinema_management_service_t service, struct book_seat_request request, struct book_seat_response* response);

struct cancel_booking_request { int32_t booking_code; };
extern errno_t cancel_booking(cinema_management_service_t service, struct cancel_booking_request request);

struct validate_booking_request { int32_t booking_code; };
struct validate_booking_response { struct response; };
extern errno_t validate_booking(cinema_management_service_t service, struct validate_booking_request request, struct validate_booking_response** response);
