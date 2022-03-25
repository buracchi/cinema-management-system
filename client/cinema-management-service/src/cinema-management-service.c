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

static inline errno_t connect(cinema_management_service_t service, const char* username, const char* password);

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

static inline errno_t connect(cinema_management_service_t service, const char* username, const char* password) {
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

extern MYSQL_STMT* get_prepared_stmt(cinema_management_service_t service, enum statement_operation operation) {
    MYSQL_STMT** stmt = &(service->statements_data[operation].statement);
    if (!*stmt) {
        const char* query = service->statements_data[operation].query;
        bool update_length = true;
        try(*stmt = mysql_stmt_init(service->db_connection), NULL, fail);
        try((mysql_stmt_prepare(*stmt, query, (unsigned long)strlen(query))) != 0, true, fail2);
        mysql_stmt_attr_set(*stmt, STMT_ATTR_UPDATE_MAX_LENGTH, &update_length);
    }
    return *stmt;
fail2:
    mysql_stmt_close(*stmt);
    *stmt = NULL;
fail:
    return *stmt;
}

extern inline const char* get_last_error(cinema_management_service_t service) {
    return mysql_error(service->db_connection);
}
