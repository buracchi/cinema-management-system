#pragma once

#include <stdint.h>
#include <errno.h>

#include "cinema-management-service.h"
#include "domain/screening.h"

struct get_screenings_response { struct screening_t* screenings; };
extern errno_t get_screenings(cinema_management_service_t service, struct get_screenings_response* response);

struct add_screening_request { int32_t cinema_id; uint8_t hall_number; char day[11]; char start_time[9]; uint64_t price_decimal; uint8_t price_cents; int32_t film_id; };
extern errno_t add_screening(cinema_management_service_t service, struct add_screening_request request);

struct get_available_projectionists_request { int32_t employee_id; char day[11]; char start_time[9]; char duration[9]; int32_t cinema_id; };
struct get_available_projectionists_response { employee_t* employees; };
extern errno_t get_available_projectionists(cinema_management_service_t service, struct get_available_projectionists_request request, struct get_available_projectionists_response* response);

struct add_screening_request { int32_t employee_id; int32_t cinema_id; uint8_t hall_number; char day[11]; char start_time[9]; };
extern errno_t assign_projectionist(cinema_management_service_t service, struct add_screening_request request);

struct delete_screening_request { int32_t employee_id; char day[11]; char start_time[9]; };
extern errno_t delete_screening(cinema_management_service_t service, struct delete_screening_request request);
