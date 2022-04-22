#pragma once

#include <stdint.h>
#include <stdbool.h>

#include "cms.h"
#include "domain/shift.h"

struct cms_get_shifts_response { struct cms_result_response; struct shift_t* shifts; };
extern bool cms_get_shifts(cms_t cms, struct get_shifts_response** response);

struct cms_add_shift_request { int32_t employee_id; char day[DATE_LEN]; char start_time[TIME_LEN]; char duration[TIME_LEN]; int32_t cinema_id; };
extern bool cms_add_shift(cms_t cms, struct add_shift_request request);

struct cms_delete_shift_request { int32_t employee_id; char day[DATE_LEN]; char start_time[TIME_LEN]; };
extern bool cms_delete_shift(cms_t cms, struct cms_delete_shift_request request);
