#include "booking.h"

#include <string.h>
#include <buracchi/common/utilities/try.h>
#include <buracchi/common/utilities/utilities.h>

#include "prepared-statement.h"
#include "utilities/dbutil.h"

extern errno_t get_all_cinema(cinema_management_service_t service, struct get_all_cinema_response* response) {
	return 0;
}

extern errno_t get_cinema_screenings(cinema_management_service_t service, struct get_cinema_screenings_request request, struct get_cinema_screenings_response* response) {
	return 0;
}

extern errno_t get_avalilable_seats(cinema_management_service_t service, struct get_avalilable_seats_request request, struct get_avalilable_seats_response* response) {
	return 0;
}

extern errno_t book_seat(cinema_management_service_t service, struct book_seat_request request, struct book_seat_response** response) {
	MYSQL_STMT* statement;
	MYSQL_TIME date;
	MYSQL_TIME start_time;
	MYSQL_TIME expiry_date;
	date_to_mysql_time(request.date, &date);
	time_to_mysql_time(request.start_time, &start_time);
	date_to_mysql_time(request.expiry_date, &expiry_date);
	MYSQL_BIND binding_params[10] = {
		{.buffer_type = MYSQL_TYPE_LONG, .buffer = &request.cinema_id,		.buffer_length = sizeof(request.cinema_id) },
		{.buffer_type = MYSQL_TYPE_LONG, .buffer = &request.hall_id,		.buffer_length = sizeof(request.hall_id) },
		{.buffer_type = MYSQL_TYPE_LONG, .buffer = &date,					.buffer_length = sizeof(date) },
		{.buffer_type = MYSQL_TYPE_LONG, .buffer = &start_time,				.buffer_length = sizeof(start_time) },
		{.buffer_type = MYSQL_TYPE_LONG, .buffer = &request.seat_row,		.buffer_length = sizeof(request.seat_row) },
		{.buffer_type = MYSQL_TYPE_LONG, .buffer = &request.seat_number,	.buffer_length = sizeof(request.seat_number) },
		{.buffer_type = MYSQL_TYPE_LONG, .buffer = &request.name_on_card,	.buffer_length = sizeof(request.name_on_card) },
		{.buffer_type = MYSQL_TYPE_LONG, .buffer = &request.card_number,	.buffer_length = sizeof(request.card_number) },
		{.buffer_type = MYSQL_TYPE_LONG, .buffer = &expiry_date,			.buffer_length = sizeof(expiry_date) },
		{.buffer_type = MYSQL_TYPE_LONG, .buffer = &request.security_code,	.buffer_length = sizeof(request.security_code) }
	};
	struct book_seat_result rset_current_row = { 0 };
	unsigned long long num_rows;
	MYSQL_BIND rparams[1] = {
		{.buffer_type = MYSQL_TYPE_LONG,	.buffer = &rset_current_row.booking_code, .buffer_length = sizeof((*response)->result->booking_code) }
	};
	try(statement = get_prepared_stmt(service, BOOK_SEAT), NULL, fail);
	try((mysql_stmt_bind_param(statement, binding_params) == 0), false, fail);
	try((mysql_stmt_execute(statement) == 0), false, fail2);
	mysql_stmt_bind_result(statement, rparams);
	mysql_stmt_store_result(statement);
	num_rows = mysql_stmt_num_rows(statement);
	try(*response = malloc(sizeof * *response + sizeof * (*response)->result * num_rows), NULL, fail2);
	(*response)->error_message = NULL;
	(*response)->num_elements = num_rows;
	for (uint64_t i = 0; i < num_rows; i++) {
		try((mysql_stmt_fetch(statement) == 0), false, fail3);
		memcpy(&((*response)->result[i]), &rset_current_row, sizeof * (*response)->result);
	}
	while (mysql_stmt_next_result(statement) != -1);
	mysql_stmt_free_result(statement);
	mysql_stmt_reset(statement);
	return 0;
fail3:
	free(*response);
fail2:
	mysql_stmt_free_result(statement);
	mysql_stmt_reset(statement);
fail:
	*response = malloc(sizeof **response);
	asprintf(&((*response)->error_message), "%s", get_last_error(service));
	(*response)->num_elements = 0;
	return 1;
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
	*response = malloc(sizeof **response);
	asprintf(&((*response)->error_message), "%s", mysql_stmt_error(statement));
	mysql_stmt_free_result(statement);
	mysql_stmt_reset(statement);
	return 1;
fail:
	*response = malloc(sizeof **response);
	asprintf(&((*response)->error_message), "%s", get_last_error(service));
	return 1;
}
