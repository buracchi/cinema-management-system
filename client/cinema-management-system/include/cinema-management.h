#pragma once

#include <stdint.h>
#include <errno.h>

#include "cinema-management-system.h"
#include "domain/cinema.h"
#include "domain/hall.h"

struct get_all_cinema_response { cinema_t* cinema; };
extern errno_t get_all_cinema(cinema_management_system_t system, struct get_all_cinema_response* response);

struct add_cinema_request { const char* address; char[9] opening_time; char[9] closing_time; };
extern errno_t add_cinema(cinema_management_system_t system, struct add_cinema_request request);

struct delete_cinema_request { int32_t id; };
extern errno_t delete_cinema(cinema_management_system_t system, struct delete_cinema_request request);

struct get_halls_request { int32_t cinema_id; };
struct get_halls_response { hall_t* halls; };
extern errno_t get_halls(cinema_management_system_t system, struct get_halls_request request, struct get_halls_response* response);

struct add_hall_request { int32_t cinema_id; uint8_t hall_number; uint8_t rows; uint8_t rows_seats; };
extern errno_t add_hall(cinema_management_system_t system, struct add_hall_request request);

struct delete_hall_request { int32_t cinema_id; uint8_t hall_number; };
extern errno_t delete_hall(cinema_management_system_t system, struct delete_hall_request request);
