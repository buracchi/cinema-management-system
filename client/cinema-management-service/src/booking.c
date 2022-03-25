#include "booking.h"

#include <buracchi/common/utilities/try.h>

#include "prepared-statement.h"

extern int execute_prepared_stmt(cinema_management_service_t service, enum statement_operation operation, void** result_set, ...);

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

extern errno_t validate_booking(cinema_management_service_t service, struct validate_booking_request request, struct validate_booking_response** response) {
	MYSQL_STMT* statement;
	MYSQL_BIND binding_params[1] = {
		{.buffer_type = MYSQL_TYPE_LONG, .buffer = &request.booking_code, .buffer_length = sizeof(request.booking_code) }
	};
	try(statement = get_prepared_stmt(service, VALIDATE_BOOKING), NULL, fail);
	try((mysql_stmt_bind_param(statement, binding_params) == 0), false, fail);
	try((mysql_stmt_execute(statement) == 0), false, fail2);
	while (mysql_stmt_next_result(statement) != -1);
	mysql_stmt_free_result(statement);
	mysql_stmt_reset(statement);
	return 0;
fail2:
	while (mysql_stmt_next_result(statement) != -1);
	mysql_stmt_free_result(statement);
	mysql_stmt_reset(statement);
fail:
	*response = malloc(sizeof ** response);
	(*response)->error_message = get_last_error(service);
	return 1;
}
