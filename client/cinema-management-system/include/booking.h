#pragma once

#include <stdint.h>
#include <errno.h>

#include "cinema-management-system.h"
#include "domain/cinema.h"
#include "domain/employee.h"

struct get_all_cinema_response { cinema_t* cinema; };
extern errno_t get_all_cinema(cinema_management_system_t system, struct get_all_cinema_response* response);

struct get_cinema_screenings_request { const char* name; const char* surname; enum role role; };
extern errno_t get_cinema_screenings(cinema_management_system_t system, struct get_cinema_screenings_request request);

struct get_avalilable_seats_request { const char* name; const char* surname; enum role role; };
extern errno_t get_avalilable_seats(cinema_management_system_t system, struct get_avalilable_seats_request request);

struct book_seat_request { const char* name; const char* surname; enum role role; };
extern errno_t book_seat(cinema_management_system_t system, struct book_seat_request request);

struct cancel_booking_request { int32_t booking_code; };
extern errno_t cancel_booking(cinema_management_system_t system, struct cancel_booking_request request);

struct validate_booking_request { int32_t booking_code; };
struct validate_booking_response { void* param; };
extern errno_t validate_booking(cinema_management_system_t system, struct validate_booking_request request, struct validate_booking_response* response);
