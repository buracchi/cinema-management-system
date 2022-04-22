#pragma once

#include <stdint.h>
#include <stdbool.h>

#include "cms.h"
#include "domain/screening.h"

struct cms_get_screenings_response { struct screening_t* screenings; };
extern bool cms_get_screenings(cms_t cms, struct cms_get_screenings_response** response);

struct cms_add_screening_request { int32_t cinema_id; uint8_t hall_number; char day[DATE_LEN]; char start_time[TIME_LEN]; char price[18]; int32_t film_id; };
extern bool cms_add_screening(cms_t cms, struct cms_add_screening_request request);

struct cms_get_available_projectionists_request { int32_t employee_id; char day[DATE_LEN]; char start_time[TIME_LEN]; char duration[TIME_LEN]; int32_t cinema_id; };
struct cms_get_available_projectionists_response { employee_t* employees; };
extern bool cms_get_available_projectionists(cms_t cms, struct cms_get_available_projectionists_request request, struct cms_get_available_projectionists_response** response);

struct cms_add_screening_request { int32_t employee_id; int32_t cinema_id; uint8_t hall_number; char day[DATE_LEN]; char start_time[TIME_LEN]; };
extern bool cms_assign_projectionist(cms_t cms, struct cms_cms_add_screening_request request);

struct cms_delete_screening_request { int32_t employee_id; char day[11]; char start_time[9]; };
extern bool cms_delete_screening(cms_t cms, struct cms_delete_screening_request request);
