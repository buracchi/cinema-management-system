#pragma once

#include <stdint.h>
#include <errno.h>

#include "cinema-management-system.h"
#include "domain/shift.h"

struct get_shifts_response { struct shift_t* shifts; };
extern errno_t get_shifts(cinema_management_system_t system, struct get_shifts_response* response);

struct add_shift_request { int32_t employee_id; char day[11]; char start_time[9]; char duration[9]; int32_t cinema_id; };
extern errno_t add_shift(cinema_management_system_t system, struct add_shift_request request);

struct delete_shift_request { int32_t employee_id; char day[11]; char start_time[9]; };
extern errno_t delete_shift(cinema_management_system_t system, struct delete_shift_request request);
