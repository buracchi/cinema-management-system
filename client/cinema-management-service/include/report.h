#pragma once

#include <stdint.h>
#include <errno.h>

#include "cinema-management-service.h"
#include "domain/cinema.h"
#include "domain/screening.h"
#include "domain/weekday.h"

struct screening_without_projectionist_report {
	cinema_t cinema;
	enum weekday weekday;
	char from_hours[9];
	char to_hours[9];
	uint8_t ushers
};

struct report_reservations_status_response { int32_t cinema_id; uint8_t hall_number; uint32_t confirmed; uint32_t canceled; uint32_t validated; uint32_t expired; };
extern errno_t report_reservations_status(cinema_management_service_t service, struct report_reservations_status_response* response);

struct report_screenings_without_projectionist_response { screening_t* screenings; };
extern errno_t report_screenings_without_projectionist(cinema_management_service_t service, struct report_screenings_without_projectionist_response* response);

struct report_screenings_without_projectionist_response { struct screening_without_projectionist_report* screenings_without_projectionist_report; };
extern errno_t report_cinema_without_ushers(cinema_management_service_t service, struct report_screenings_without_projectionist_response* response);
