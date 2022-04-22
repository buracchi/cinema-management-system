#include "cms.h"

#include <mysql.h>
#include <buracchi/common/utilities/utilities.h>
#include <buracchi/common/utilities/try.h>

#include "prepared-statement.h"
#include "utilities/dbutil.h"

struct cms {
    MYSQL* db_connection;
    struct statement_data statements_data[PREPARED_STATEMENT_NUMBER];
};

static inline bool connect(cms_t cms, const char* username, const char* password);

extern cms_t cms_init(const char* username, const char* password) {
    cms_t this;
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

extern bool cms_destroy(cms_t cms) {
    for (int i = 0; i < PREPARED_STATEMENT_NUMBER; i++) {
        MYSQL_STMT* statement = cms->statements_data[i].statement;
        if (statement) {
            mysql_stmt_close(statement);
        }
    }
    mysql_close(cms->db_connection);
    free(cms);
    return true;
}

extern inline void cms_destroy_response(struct cms_response* response) {
    free(response->error_message);
    free(response);
}

static inline bool connect(cms_t cms, const char* username, const char* password) {
    unsigned int timeout = 30;
    bool reconnect = true;
    char* host = getenv("HOST");
    char* db = getenv("DB");
    unsigned int port = atoi(getenv("PORT"));
    mysql_real_connect(cms->db_connection, host, username, password, db, port, NULL,
        CLIENT_MULTI_STATEMENTS | CLIENT_MULTI_RESULTS | CLIENT_COMPRESS | CLIENT_INTERACTIVE | CLIENT_REMEMBER_OPTIONS);
    mysql_options(cms->db_connection, MYSQL_OPT_CONNECT_TIMEOUT, &timeout);
    mysql_options(cms->db_connection, MYSQL_OPT_RECONNECT, &reconnect);
    return true;
}

extern MYSQL_STMT* get_prepared_stmt(cms_t cms, enum statement_operation operation) {
    MYSQL_STMT** stmt = &(cms->statements_data[operation].statement);
    if (!*stmt) {
        const char* query = cms->statements_data[operation].query;
        bool update_length = true;
        try(*stmt = mysql_stmt_init(cms->db_connection), NULL, fail);
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

extern inline const char* cms_get_error_message(cms_t cms) {
    return mysql_error(cms->db_connection);
}
