#include "dbutil.h"

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <mysql.h>
#include <stdarg.h>

#include <buracchi/common/utilities/try.h>

void print_stmt_error (MYSQL_STMT *stmt, char *message) {
	fprintf (stderr, "%s\n", message);
	if (stmt != NULL) {
		fprintf (stderr, "Error %u (%s): %s\n",
			 mysql_stmt_errno (stmt),
			 mysql_stmt_sqlstate(stmt),
			 mysql_stmt_error (stmt));
	}
}

void print_error(MYSQL *conn, char *message) {
	fprintf (stderr, "%s\n", message);
	if (conn != NULL) {
#if MYSQL_VERSION_ID >= 40101
		fprintf (stderr, "Error %u (%s): %s\n",
			 mysql_errno (conn), mysql_sqlstate(conn), mysql_error (conn));
#else
		fprintf (stderr, "Error %u: %s\n",
		mysql_errno (conn), mysql_error (conn));
#endif
	}
}


bool setup_prepared_stmt(MYSQL_STMT **stmt, const char *statement, MYSQL *conn) {
	bool update_length = true;
	*stmt = mysql_stmt_init(conn);
	if (*stmt == NULL) {
		print_error(conn, "Could not initialize statement handler");
		return false;
	}
	if (mysql_stmt_prepare(*stmt, statement, (unsigned long)strlen(statement)) != 0) {
		print_stmt_error(*stmt, "Could not prepare statement");
		return false;
	}
	mysql_stmt_attr_set(*stmt, STMT_ATTR_UPDATE_MAX_LENGTH, &update_length);
	return true;
}

extern bool execute_prepared_stmt(struct mysql_statement_data* statement_data, void** result_set, ...) {
	va_list params;
	MYSQL_BIND* binding_params;
	MYSQL_TIME* timebased_params;
	try(binding_params = malloc(sizeof * binding_params * statement_data->required_params), NULL, fail);
	try(timebased_params = malloc(sizeof * timebased_params * statement_data->required_params), NULL, fail);
	va_start(params, result_set);
	for (int i = 0; i < statement_data->required_params; i++) {
		switch (statement_data->param_types[i]) {
		case FIELD_TYPE_DECIMAL:
			set_binding_param(&binding_params[i], MYSQL_TYPE_DECIMAL,	va_arg(params, char*),			sizeof(char*) * 65);
			break;
		case FIELD_TYPE_TINY:
			set_binding_param(&binding_params[i], MYSQL_TYPE_TINY,		va_arg(params, signed char*),	sizeof(signed char));
			break;
		case FIELD_TYPE_SHORT:
			set_binding_param(&binding_params[i], MYSQL_TYPE_SHORT,		va_arg(params, short int*),		sizeof(short int));
			break;
		case FIELD_TYPE_LONG:
			set_binding_param(&binding_params[i], MYSQL_TYPE_LONG,		va_arg(params, int*),			sizeof(int));
			break;
		case FIELD_TYPE_LONGLONG:
			set_binding_param(&binding_params[i], MYSQL_TYPE_LONGLONG,	va_arg(params, long long int*),	sizeof(long long int));
			break;
		case FIELD_TYPE_FLOAT:
			set_binding_param(&binding_params[i], MYSQL_TYPE_FLOAT,		va_arg(params, float*),			sizeof(float));
			break;
		case FIELD_TYPE_DOUBLE:
			set_binding_param(&binding_params[i], MYSQL_TYPE_DOUBLE,	va_arg(params, double*),			sizeof(double));
			break;
		case FIELD_TYPE_TIME:
			time_to_mysql_time(va_arg(params, char*), &timebased_params[i]);
			set_binding_param(&binding_params[i], MYSQL_TYPE_TIME,		&timebased_params[i],			sizeof(MYSQL_TIME));
			break;
		case FIELD_TYPE_DATE:
			date_to_mysql_time(va_arg(params, char*), &timebased_params[i]);
			set_binding_param(&binding_params[i], MYSQL_TYPE_DATE,		&timebased_params[i],			sizeof(MYSQL_TIME));
			break;
		case FIELD_TYPE_DATETIME:
			// TODO
			//set_binding_param(&binding_params[i], MYSQL_TYPE_DATETIME, va_arg(params, char*), sizeof(MYSQL_TIME));
			//break;
			goto unimplemented;
		case FIELD_TYPE_TIMESTAMP:
			// TODO
			//set_binding_param(&binding_params[i], MYSQL_TYPE_TIMESTAMP, va_arg(params, char*), sizeof(MYSQL_TIME));
			//break;
			goto unimplemented;
		case FIELD_TYPE_STRING: {
			char* param = va_arg(params, char*);
			set_binding_param(&binding_params[i], MYSQL_TYPE_STRING,	param,							(unsigned long)strlen(param));
		}
			break;
		case FIELD_TYPE_BLOB:
			//set_binding_param(&binding_params[i], MYSQL_TYPE_BLOB, va_arg(params, char*), sizeof(char*));
			//break;
			goto unimplemented;
		case FIELD_TYPE_NULL:
			//set_binding_param(&binding_params[i], MYSQL_TYPE_NULL, va_arg(params, void*), sizeof(void*));
			//break;
			goto unimplemented;
unimplemented:
		default:
			goto fail2;
		}
	}
	va_end(params);
	try(mysql_stmt_bind_param(statement_data->statement, binding_params), !0, fail2);
	try(mysql_stmt_execute(statement_data->statement), !0, fail2);
	mysql_stmt_free_result(statement_data->statement);
	mysql_stmt_reset(statement_data->statement);
	free(timebased_params);
	free(binding_params);
	return true;
fail2:
	free(binding_params);
fail:
	return false;
}


void finish_with_error(MYSQL *conn, char *message) {
	print_error(conn, message);
	mysql_close(conn);
	exit(EXIT_FAILURE);
}


void finish_with_stmt_error(MYSQL *conn, MYSQL_STMT *stmt, char *message, bool close_stmt) {
	print_stmt_error(stmt, message);
	if(close_stmt)
		mysql_stmt_close(stmt);
	mysql_close(conn);
	exit(EXIT_FAILURE);
}

void set_binding_param(MYSQL_BIND *param, enum enum_field_types type, void *buffer, unsigned long len) {
	memset(param, 0, sizeof(*param));
	param->buffer_type = type;
	param->buffer = buffer;
	param->buffer_length = len;
}


void date_to_mysql_time(char *str, MYSQL_TIME *time) {
	memset(time, 0, sizeof(*time));
	sscanf(str, "%4d-%2d-%2d", &time->year, &time->month, &time->day);
	time->time_type = MYSQL_TIMESTAMP_DATE;
}


void time_to_mysql_time(char *str, MYSQL_TIME *time) {
	memset(time, 0, sizeof(*time));
	sscanf(str, "%02d:%02d", &time->hour, &time->minute);
	time->time_type = MYSQL_TIMESTAMP_TIME;
}

void init_mysql_timestamp(MYSQL_TIME *time) {
	memset(time, 0, sizeof (*time));
	time->time_type = MYSQL_TIMESTAMP_DATETIME;
}

void mysql_timestamp_to_string(MYSQL_TIME *time, char *str) {
	snprintf(str, DATETIME_LEN, "%4d-%02d-%02d %02d:%02d", time->year, time->month, time->day, time->hour, time->minute);
}

void mysql_date_to_string(MYSQL_TIME *date, char *str) {
	snprintf(str, DATE_LEN, "%4d-%02d-%02d", date->year, date->month, date->day);
}
