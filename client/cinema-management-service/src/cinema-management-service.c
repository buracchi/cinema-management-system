#include "cinema-management-service.h"

#include <mysql.h>
#include <buracchi/common/utilities/utilities.h>
#include <buracchi/common/utilities/try.h>

#include "prepared-statement.h"
#include "utilities/dbutil.h"

struct cinema_management_service {
    MYSQL* db_connection;
    struct statement_data statements_data[PREPARED_STATEMENT_NUMBER];
};


static errno_t connect(cinema_management_service_t service, const char* username, const char* password);
extern int execute_prepared_stmt(cinema_management_service_t service, enum statement_operation operation, void** result_set, ...);
static MYSQL_STMT* get_prepared_stmt(cinema_management_service_t service, enum statement_operation operation);
static int bind_stmt_param(cinema_management_service_t service, enum statement_operation operation, MYSQL_BIND** binding_params, MYSQL_TIME** timebased_params, va_list params);

extern cinema_management_service_t cinema_management_service_init(const char* username, const char* password) {
    cinema_management_service_t this;
    try(this = malloc(sizeof * this), NULL, fail);
    try(this->db_connection = mysql_init(NULL), NULL, fail2);
    connect(this, username, password);
    memcpy(this->statements_data, statements_data, sizeof(statements_data));
    return this;
fail2:
    free(this);
fail:
    return NULL;
}

static errno_t connect(cinema_management_service_t service, const char* username, const char* password) {
    unsigned int timeout = 30;
    bool reconnect = true;
    char* host = getenv("HOST");
    char* db = getenv("DB");
    unsigned int port = atoi(getenv("PORT"));
    mysql_real_connect(service->db_connection, host, username, password, db, port, NULL,
        CLIENT_MULTI_STATEMENTS | CLIENT_MULTI_RESULTS | CLIENT_COMPRESS | CLIENT_INTERACTIVE | CLIENT_REMEMBER_OPTIONS);
    mysql_options(service->db_connection, MYSQL_OPT_CONNECT_TIMEOUT, &timeout);
    mysql_options(service->db_connection, MYSQL_OPT_RECONNECT, &reconnect);
    return 0;
}

extern errno_t cinema_management_service_destroy(cinema_management_service_t service) {
    for (int i = 0; i < PREPARED_STATEMENT_NUMBER; i++) {
        MYSQL_STMT* statement = service->statements_data[i].statement;
        if (statement) {
            mysql_stmt_close(statement);
        }
    }
    mysql_close(service->db_connection);
    free(service);
    return 0;
}

extern int execute_prepared_stmt(cinema_management_service_t service, enum statement_operation operation, void** result_set, ...) {
    MYSQL_STMT* statement;
    MYSQL_BIND* binding_params = NULL;
    MYSQL_TIME* timebased_params = NULL;
    va_list params;
    va_start(params, result_set);
    statement = get_prepared_stmt(service, operation);
    if (!statement) {
        // return some kind of error
    }
    if (bind_stmt_param(service, operation, &binding_params, &timebased_params, params)) {
        // return some kind of error
    }
    if (mysql_stmt_execute(statement)) {
        // return some kind of error
    }
    // set result_set variable
    mysql_stmt_free_result(statement);
    mysql_stmt_reset(statement);
    free(binding_params);
    free(timebased_params);
    va_end(params);
    return 0;
}

static MYSQL_STMT* get_prepared_stmt(cinema_management_service_t service, enum statement_operation operation) {
    MYSQL_STMT** stmt = &(service->statements_data[operation].statement);
    const char* query = service->statements_data[operation].query;
    if (!*stmt) {
        bool update_length = true;
        try(*stmt = mysql_stmt_init(service->db_connection), NULL, fail);
        try(mysql_stmt_prepare(*stmt, query, (unsigned long)strlen(query)) != 0, true, fail2);
        mysql_stmt_attr_set(*stmt, STMT_ATTR_UPDATE_MAX_LENGTH, &update_length);
    }
    return *stmt;
fail2:
    mysql_stmt_close(*stmt);
    *stmt = NULL;
fail:
    return *stmt;
}

static int bind_stmt_param(cinema_management_service_t service, enum statement_operation operation, MYSQL_BIND** binding_params, MYSQL_TIME** timebased_params, va_list params) {
    uint8_t required_params = service->statements_data[operation].required_params;
    try(*binding_params = malloc(sizeof **binding_params * required_params), NULL, fail);
    try(*timebased_params = malloc(sizeof **timebased_params * required_params), NULL, fail2);
    for (int i = 0; i < required_params; i++) {
        switch (service->statements_data[operation].params_type[i]) {
        case FIELD_TYPE_DECIMAL:
            set_binding_param(&((*binding_params)[i]), MYSQL_TYPE_DECIMAL,	va_arg(params, char*),          sizeof(char*) * 65);
            break;
        case FIELD_TYPE_TINY:
            set_binding_param(&((*binding_params)[i]), MYSQL_TYPE_TINY,		va_arg(params, signed char*),	sizeof(signed char));
            break;
        case FIELD_TYPE_SHORT:
            set_binding_param(&((*binding_params)[i]), MYSQL_TYPE_SHORT,	va_arg(params, short int*),		sizeof(short int));
            break;
        case FIELD_TYPE_LONG:
            set_binding_param(&((*binding_params)[i]), MYSQL_TYPE_LONG,		va_arg(params, int*),			sizeof(int));
            break;
        case FIELD_TYPE_LONGLONG:
            set_binding_param(&((*binding_params)[i]), MYSQL_TYPE_LONGLONG, va_arg(params, long long int*),	sizeof(long long int));
            break;
        case FIELD_TYPE_FLOAT:
            set_binding_param(&((*binding_params)[i]), MYSQL_TYPE_FLOAT,	va_arg(params, float*),			sizeof(float));
            break;
        case FIELD_TYPE_DOUBLE:
            set_binding_param(&((*binding_params)[i]), MYSQL_TYPE_DOUBLE,	va_arg(params, double*),        sizeof(double));
            break;
        case FIELD_TYPE_TIME:
            time_to_mysql_time(va_arg(params, char*), &((*timebased_params)[i]));
            set_binding_param(&((*binding_params)[i]), MYSQL_TYPE_TIME,     &(timebased_params[i]),			sizeof(MYSQL_TIME));
            break;
        case FIELD_TYPE_DATE:
            date_to_mysql_time(va_arg(params, char*), &((*timebased_params)[i]));
            set_binding_param(&((*binding_params)[i]), MYSQL_TYPE_DATE,     &(timebased_params[i]),         sizeof(MYSQL_TIME));
            break;
        case FIELD_TYPE_DATETIME:
            goto unimplemented;
        case FIELD_TYPE_TIMESTAMP:
            goto unimplemented;
        case FIELD_TYPE_STRING: {
            char* param = va_arg(params, char*);
            set_binding_param(&((*binding_params)[i]), MYSQL_TYPE_STRING,	param,                          (unsigned long)strlen(param));
        }
            break;
        case FIELD_TYPE_BLOB:
            goto unimplemented;
        case FIELD_TYPE_NULL:
            goto unimplemented;
unimplemented:
        default:
            goto fail3;
        }
    }
    try(mysql_stmt_bind_param(service->statements_data[operation].statement, *binding_params), !0, fail3);
    return 0;
fail3:
    free(*timebased_params);
    *timebased_params = NULL;
fail2:
    free(*binding_params);
    *binding_params = NULL;
fail:
    return 1;
}
