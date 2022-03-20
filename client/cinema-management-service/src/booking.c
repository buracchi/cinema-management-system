#pragma once

#include "booking.h"

extern errno_t get_all_cinema(cinema_management_service_t service, struct get_all_cinema_response* response) {
	return 0;
}

extern errno_t get_cinema_screenings(cinema_management_service_t service, struct get_cinema_screenings_request request, struct get_cinema_screenings_response* response) {
	return 0;
}

extern errno_t get_avalilable_seats(cinema_management_service_t service, struct get_avalilable_seats_request request, struct get_avalilable_seats_response* response) {
	return 0;
}

extern errno_t book_seat(cinema_management_service_t service, struct book_seat_request request, struct book_seat_response* response) {
	return 0;
}

extern errno_t cancel_booking(cinema_management_service_t service, struct cancel_booking_request request) {
	return 0;
}

extern errno_t validate_booking(cinema_management_service_t service, struct validate_booking_request request, struct validate_booking_response* response) {
	return 0;
}
