#pragma once
#pragma warning(disable:4996)

#include <stdint.h>
#include <stdbool.h>
#include <mysql.h>

#define FIELD_TYPE enum enum_field_types

#define DATE_LEN 11
#define TIME_LEN 6
#define DATETIME_LEN (DATE_LEN + TIME_LEN)

struct mysql_statement_data {
	MYSQL_STMT* statement;
	uint8_t required_params;
	FIELD_TYPE* param_types;
	uint8_t result_columns;
	FIELD_TYPE* result_columns_type;
};

extern void print_stmt_error (MYSQL_STMT *stmt, char *message);
extern void print_error(MYSQL *conn, char *message);
extern bool setup_prepared_stmt(MYSQL_STMT **stmt, const char *statement, MYSQL *conn);
extern bool execute_prepared_stmt(struct mysql_statement_data* statement_data, void** result_set, ...);
extern void finish_with_error(MYSQL *conn, char *message);
extern void finish_with_stmt_error(MYSQL *conn, MYSQL_STMT *stmt, char *message, bool close_stmt);
extern void set_binding_param(MYSQL_BIND *param, enum enum_field_types type, void *buffer, unsigned long len);
extern void date_to_mysql_time(char *str, MYSQL_TIME *time);
extern void time_to_mysql_time(char *str, MYSQL_TIME *time);
extern void init_mysql_timestamp(MYSQL_TIME *time);
extern void mysql_timestamp_to_string(MYSQL_TIME *time, char *str);
extern void mysql_date_to_string(MYSQL_TIME *date, char *str);
