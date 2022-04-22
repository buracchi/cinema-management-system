#include "booking.h"

#include <string.h>
#include <buracchi/common/utilities/try.h>
#include <buracchi/common/utilities/utilities.h>

#include "prepared-statement.h"
#include "utilities/dbutil.h"

extern bool cms_get_all_cinema(cms_t cms, struct cms_get_all_cinema_response** response) {
	MYSQL_STMT* statement;
	MYSQL_TIME opening;
	MYSQL_TIME closing;
	struct cms_cinema rset_current_row = { 0 };
	unsigned long long num_rows;
	MYSQL_BIND rparams[4] = {
		{.buffer_type = MYSQL_TYPE_LONG,		.buffer = &rset_current_row.id,			.buffer_length = sizeof((*response)->result->id) },
		{.buffer_type = MYSQL_TYPE_VAR_STRING,	.buffer = &rset_current_row.address,	.buffer_length = sizeof((*response)->result->address) },
		{.buffer_type = MYSQL_TYPE_TIME,		.buffer = &opening,						.buffer_length = sizeof(opening) },
		{.buffer_type = MYSQL_TYPE_TIME,		.buffer = &closing,						.buffer_length = sizeof(closing) }
	};
	try(statement = get_prepared_stmt(cms, GET_ALL_CINEMA), NULL, fail);
	try((mysql_stmt_execute(statement) == 0), false, fail2);
	try((mysql_stmt_bind_result(statement, rparams) == 0), false, fail2);
	mysql_stmt_store_result(statement);
	num_rows = mysql_stmt_num_rows(statement);
	try(*response = malloc(sizeof **response + sizeof *((*response)->result) * num_rows), NULL, fail2);
	(*response)->error_message = NULL;
	(*response)->num_elements = num_rows;
	for (uint64_t i = 0; i < num_rows; i++) {
		try((mysql_stmt_fetch(statement) == 0), false, fail3);
		mysql_time_to_string(&opening, &(rset_current_row.opening_time[0]));
		mysql_time_to_string(&closing, &(rset_current_row.closing_time[0]));
		memcpy(&((*response)->result[i]), &rset_current_row, sizeof *(*response)->result);
	}
	while (mysql_stmt_next_result(statement) != -1);
	mysql_stmt_free_result(statement);
	mysql_stmt_reset(statement);
	return true;
fail3:
	free(*response);
fail2:
	*response = malloc(sizeof * *response);
	asprintf(&((*response)->error_message), "%s", mysql_stmt_error(statement));
	mysql_stmt_free_result(statement);
	mysql_stmt_reset(statement);
	return false;
fail:
	*response = malloc(sizeof * *response);
	asprintf(&((*response)->error_message), "%s", cms_get_error_message(cms));
	(*response)->num_elements = 0;
	return false;
}

extern bool cms_get_cinema_screenings(cms_t cms, struct cms_get_cinema_screenings_request request, struct cms_get_cinema_screenings_response** response) {
	MYSQL_STMT* statement;
	MYSQL_TIME date;
	MYSQL_TIME time;
	MYSQL_TIME running_time;
	MYSQL_BIND binding_params[1] = {
		{.buffer_type = MYSQL_TYPE_LONG,		.buffer = &request.cinema_id,				.buffer_length = sizeof(request.cinema_id) }
	};
	struct cms_screening rset_current_row = { 0 };
	unsigned long long num_rows;
	MYSQL_BIND rparams[9] = {
		{.buffer_type = MYSQL_TYPE_DATE,		.buffer = &date,							.buffer_length = sizeof(date) },
		{.buffer_type = MYSQL_TYPE_TIME,		.buffer = &time,							.buffer_length = sizeof(time) },
		{.buffer_type = MYSQL_TYPE_LONG,		.buffer = &rset_current_row.cinema_id,		.buffer_length = sizeof((*response)->result->cinema_id) },
		{.buffer_type = MYSQL_TYPE_LONG,		.buffer = &rset_current_row.hall_id,		.buffer_length = sizeof((*response)->result->hall_id) },
		{.buffer_type = MYSQL_TYPE_NEWDECIMAL,	.buffer = &rset_current_row.price,			.buffer_length = sizeof((*response)->result->price) },
		{.buffer_type = MYSQL_TYPE_VAR_STRING,	.buffer = &rset_current_row.film_name,		.buffer_length = sizeof((*response)->result->film_name) },
		{.buffer_type = MYSQL_TYPE_TIME,		.buffer = &running_time,					.buffer_length = sizeof(running_time) },
		{.buffer_type = MYSQL_TYPE_VAR_STRING,	.buffer = &rset_current_row.film_studio,	.buffer_length = sizeof((*response)->result->film_studio) },
		{.buffer_type = MYSQL_TYPE_VAR_STRING,	.buffer = &rset_current_row.cast,			.buffer_length = sizeof((*response)->result->cast) }
	};
	try(statement = get_prepared_stmt(cms, GET_CINEMA_SCREENINGS), NULL, fail);
	try((mysql_stmt_bind_param(statement, binding_params) == 0), false, fail);
	try((mysql_stmt_execute(statement) == 0), false, fail2);
	try((mysql_stmt_bind_result(statement, rparams) == 0), false, fail2);
	mysql_stmt_store_result(statement);
	num_rows = mysql_stmt_num_rows(statement);
	try(*response = malloc(sizeof * *response + sizeof * (*response)->result * num_rows), NULL, fail2);
	(*response)->error_message = NULL;
	(*response)->num_elements = num_rows;
	for (uint64_t i = 0; i < num_rows; i++) {
		try((mysql_stmt_fetch(statement) == 0), false, fail3);
		mysql_date_to_string(&date, &(rset_current_row.date[0]));
		mysql_time_to_string(&time, &(rset_current_row.time[0]));
		mysql_time_to_string(&running_time, &(rset_current_row.running_time[0]));
		memcpy(&((*response)->result[i]), &rset_current_row, sizeof * (*response)->result);
	}
	while (mysql_stmt_next_result(statement) != -1);
	mysql_stmt_free_result(statement);
	mysql_stmt_reset(statement);
	return true;
fail3:
	free(*response);
fail2:
	*response = malloc(sizeof * *response);
	asprintf(&((*response)->error_message), "%s", mysql_stmt_error(statement));
	mysql_stmt_free_result(statement);
	mysql_stmt_reset(statement);
	return false;
fail:
	*response = malloc(sizeof * *response);
	asprintf(&((*response)->error_message), "%s", cms_get_error_message(cms));
	(*response)->num_elements = 0;
	return false;
}

extern bool cms_get_available_seats(cms_t cms, struct cms_get_available_seats_request request, struct cms_get_available_seats_response** response) {
	MYSQL_STMT* statement;
	MYSQL_TIME date;
	MYSQL_TIME start_time;
	date_to_mysql_time(request.date, &date);
	time_to_mysql_time(request.start_time, &start_time);
	MYSQL_BIND binding_params[4] = {
		{.buffer_type = MYSQL_TYPE_LONG,		.buffer = &request.cinema_id,				.buffer_length = sizeof(request.cinema_id) },
		{.buffer_type = MYSQL_TYPE_LONG,		.buffer = &request.hall_id,					.buffer_length = sizeof(request.cinema_id) },
		{.buffer_type = MYSQL_TYPE_DATE,		.buffer = &date,							.buffer_length = sizeof(date) },
		{.buffer_type = MYSQL_TYPE_TIME,		.buffer = &start_time,						.buffer_length = sizeof(start_time) }
	};
	struct cms_hall_info hi_rset_current_row = { 0 };
	struct cms_seat seat_rset_current_row = { 0 };
	unsigned long long num_rows;
	MYSQL_BIND hall_info_rparams[2] = {
		{.buffer_type = MYSQL_TYPE_LONG,		.buffer = &hi_rset_current_row.num_rows,	.buffer_length = sizeof(hi_rset_current_row.num_rows) },
		{.buffer_type = MYSQL_TYPE_LONG,		.buffer = &hi_rset_current_row.num_cols,	.buffer_length = sizeof(hi_rset_current_row.num_cols) },
	};
	MYSQL_BIND seat_rparams[2] = {
		{.buffer_type = MYSQL_TYPE_STRING,		.buffer = &seat_rset_current_row.row,		.buffer_length = sizeof(seat_rset_current_row.row) },
		{.buffer_type = MYSQL_TYPE_LONG,		.buffer = &seat_rset_current_row.number,	.buffer_length = sizeof(seat_rset_current_row.number) },
	};
	try(statement = get_prepared_stmt(cms, GET_AVALILABLE_SEATS), NULL, fail);
	try((mysql_stmt_bind_param(statement, binding_params) == 0), false, fail);
	try((mysql_stmt_execute(statement) == 0), false, fail2);
	try((mysql_stmt_bind_result(statement, hall_info_rparams) == 0), false, fail2);
	try((mysql_stmt_fetch(statement) == 0), false, fail3);
	mysql_stmt_next_result(statement);
	try((mysql_stmt_bind_result(statement, seat_rparams) == 0), false, fail2);
	mysql_stmt_store_result(statement);
	num_rows = mysql_stmt_num_rows(statement);
	try(*response = malloc(sizeof * *response + sizeof * (*response)->seat_result * num_rows), NULL, fail2);
	(*response)->error_message = NULL;
	(*response)->num_elements = num_rows;
	(*response)->hall_info_result.num_rows = hi_rset_current_row.num_rows;
	(*response)->hall_info_result.num_cols = hi_rset_current_row.num_cols;
	for (uint64_t i = 0; i < num_rows; i++) {
		try((mysql_stmt_fetch(statement) == 0), false, fail3);
		memcpy(&((*response)->seat_result[i]), &seat_rset_current_row, sizeof * (*response)->seat_result);
	}
	while (mysql_stmt_next_result(statement) != -1);
	mysql_stmt_free_result(statement);
	mysql_stmt_reset(statement);
	return true;
fail3:
	free(*response);
fail2:
	*response = malloc(sizeof * *response);
	asprintf(&((*response)->error_message), "%s", mysql_stmt_error(statement));
	mysql_stmt_free_result(statement);
	mysql_stmt_reset(statement);
	return false;
fail:
	*response = malloc(sizeof * *response);
	asprintf(&((*response)->error_message), "%s", cms_get_error_message(cms));
	(*response)->num_elements = 0;
	return false;
}

extern bool cms_book_seat(cms_t cms, struct cms_book_seat_request request, struct cms_book_seat_response** response) {
	MYSQL_STMT* statement;
	MYSQL_TIME date;
	MYSQL_TIME start_time;
	MYSQL_TIME expiry_date;
	date_to_mysql_time(request.date, &date);
	time_to_mysql_time(request.start_time, &start_time);
	date_to_mysql_time(request.expiry_date, &expiry_date);
	MYSQL_BIND binding_params[10] = {
		{.buffer_type = MYSQL_TYPE_LONG,		.buffer = &request.cinema_id,				.buffer_length = sizeof(request.cinema_id) },
		{.buffer_type = MYSQL_TYPE_LONG,		.buffer = &request.hall_id,					.buffer_length = sizeof(request.hall_id) },
		{.buffer_type = MYSQL_TYPE_DATE,		.buffer = &date,							.buffer_length = sizeof(date) },
		{.buffer_type = MYSQL_TYPE_TIME,		.buffer = &start_time,						.buffer_length = sizeof(start_time) },
		{.buffer_type = MYSQL_TYPE_STRING,		.buffer = &request.seat_row,				.buffer_length = sizeof(request.seat_row) },
		{.buffer_type = MYSQL_TYPE_LONG,		.buffer = &request.seat_number,				.buffer_length = sizeof(request.seat_number) },
		{.buffer_type = MYSQL_TYPE_VAR_STRING,	.buffer = &request.name_on_card,			.buffer_length = sizeof(request.name_on_card) },
		{.buffer_type = MYSQL_TYPE_NEWDECIMAL,	.buffer = &request.card_number,				.buffer_length = sizeof(request.card_number) },
		{.buffer_type = MYSQL_TYPE_DATE,		.buffer = &expiry_date,						.buffer_length = sizeof(expiry_date) },
		{.buffer_type = MYSQL_TYPE_NEWDECIMAL,	.buffer = &request.security_code,			.buffer_length = sizeof(request.security_code) }
	};
	struct cms_book_seat_result rset_current_row = { 0 };
	unsigned long long num_rows;
	MYSQL_BIND rparams[1] = {
		{.buffer_type = MYSQL_TYPE_LONG,		.buffer = &rset_current_row.booking_code,	.buffer_length = sizeof((*response)->result->booking_code) }
	};
	try(statement = get_prepared_stmt(cms, BOOK_SEAT), NULL, fail);
	try((mysql_stmt_bind_param(statement, binding_params) == 0), false, fail);
	try((mysql_stmt_execute(statement) == 0), false, fail2);
	try((mysql_stmt_bind_result(statement, rparams) == 0), false, fail2);
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
	return true;
fail3:
	free(*response);
fail2:
	*response = malloc(sizeof * *response);
	asprintf(&((*response)->error_message), "%s", mysql_stmt_error(statement));
	mysql_stmt_free_result(statement);
	mysql_stmt_reset(statement);
	return false;
fail:
	*response = malloc(sizeof **response);
	asprintf(&((*response)->error_message), "%s", cms_get_error_message(cms));
	(*response)->num_elements = 0;
	return false;
}

extern bool cms_cancel_booking(cms_t cms, struct cms_cancel_booking_request request, struct cms_cancel_booking_response** response) {
	MYSQL_STMT* statement;
	MYSQL_BIND binding_params[1] = {
		{.buffer_type = MYSQL_TYPE_LONG, .buffer = &request.booking_code, .buffer_length = sizeof(request.booking_code) }
	};
	try(statement = get_prepared_stmt(cms, CANCEL_BOOKING), NULL, fail);
	try((mysql_stmt_bind_param(statement, binding_params) == 0), false, fail);
	try((mysql_stmt_execute(statement) == 0), false, fail2);
	while (mysql_stmt_next_result(statement) != -1);
	*response = malloc(sizeof **response);
	memset(*response, 0, sizeof(*response));
	mysql_stmt_free_result(statement);
	mysql_stmt_reset(statement);
	return true;
fail2:
	*response = malloc(sizeof * *response);
	asprintf(&((*response)->error_message), "%s", mysql_stmt_error(statement));
	mysql_stmt_free_result(statement);
	mysql_stmt_reset(statement);
	return false;
fail:
	*response = malloc(sizeof * *response);
	asprintf(&((*response)->error_message), "%s", cms_get_error_message(cms));
	return false;
}

extern bool cms_validate_booking(cms_t cms, struct cms_validate_booking_request request, struct cms_validate_booking_response** response) {
	MYSQL_STMT* statement;
	MYSQL_BIND binding_params[1] = {
		{.buffer_type = MYSQL_TYPE_LONG, .buffer = &request.booking_code, .buffer_length = sizeof(request.booking_code) }
	};
	try(statement = get_prepared_stmt(cms, VALIDATE_BOOKING), NULL, fail);
	try((mysql_stmt_bind_param(statement, binding_params) == 0), false, fail);
	try((mysql_stmt_execute(statement) == 0), false, fail2);
	while (mysql_stmt_next_result(statement) != -1);
	mysql_stmt_free_result(statement);
	mysql_stmt_reset(statement);
	return true;
fail2:
	*response = malloc(sizeof **response);
	asprintf(&((*response)->error_message), "%s", mysql_stmt_error(statement));
	mysql_stmt_free_result(statement);
	mysql_stmt_reset(statement);
	return false;
fail:
	*response = malloc(sizeof **response);
	asprintf(&((*response)->error_message), "%s", cms_get_error_message(cms));
	return false;
}
