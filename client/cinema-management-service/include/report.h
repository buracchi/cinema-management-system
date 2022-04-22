#pragma once

#include <stdint.h>
#include <stdbool.h>

#include "cms.h"
#include "domain/cinema.h"
#include "domain/screening.h"
#include "domain/weekday.h"

struct cms_screening_without_projectionist_report {
	cinema_t cinema;
	enum weekday weekday;
	char from_hours[9];
	char to_hours[9];
	uint8_t ushers
};

struct cms_report_reservations_status_response { int32_t cinema_id; uint8_t hall_number; uint32_t confirmed; uint32_t canceled; uint32_t validated; uint32_t expired; };
extern bool cms_report_reservations_status(cms_t cms, struct cms_report_reservations_status_response** response);

struct cms_report_screenings_without_projectionist_response { screening_t* screenings; };
extern bool cms_report_screenings_without_projectionist(cms_t cms, struct cms_report_screenings_without_projectionist_response** response);

struct cms_report_screenings_without_projectionist_response { struct cms_screening_without_projectionist_report* screenings_without_projectionist_report; };
extern bool cms_report_cinema_without_ushers(cms_t cms, struct cms_report_screenings_without_projectionist_response** response);
