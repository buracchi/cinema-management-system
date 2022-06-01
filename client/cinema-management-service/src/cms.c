#include "cms.h"

#include <mysql.h>
#include <buracchi/common/utilities/utilities.h>
#include <buracchi/common/utilities/try.h>

#include "cms-statement.h"
#include "utilities/dbutil.h"

struct cms {
    MYSQL* db_connection;
    struct statement_data statements_data[PREPARED_STATEMENT_NUMBER];
};

static inline bool connect(cms_t cms, const char* username, const char* password);
static int stmt_bind_param(struct statement_data statement_data, struct request_param* request_param);

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

extern inline const char* cms_get_error_message(cms_t cms) {
    return mysql_error(cms->db_connection);
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

extern int cms_stmt_execute(cms_t cms,
                            enum statement_operation operation,
                            struct request_param* request_param,
                            struct cms_result_response** response,
                            struct result_bitmap* result_bitmap) {
    MYSQL_STMT* statement;
    MYSQL_RES* result_metadata;
    MYSQL_TIME* timebased_result_params;
    unsigned int result_param_count;
    MYSQL_BIND* result_params;
    uint8_t* result;
    size_t result_offest;
    size_t result_length;
    unsigned long long rset_num_rows;
    uint8_t* rset_current_row;
    try(statement = get_prepared_stmt(cms, operation), NULL, fail);
    try(stmt_bind_param(cms->statements_data[operation], request_param), 1, fail);
    try((mysql_stmt_execute(statement) == 0), false, fail2);
    try(result_metadata = mysql_stmt_result_metadata(statement), NULL, fail);
    result_param_count = result_metadata->field_count;
    if (result_param_count) {
        result_offest = result_bitmap[0].offset;
        result_length = result_bitmap[0].size;
    }
    try(result_params = malloc(sizeof *result_params * result_param_count), NULL, fail);
    memset(result_params, 0, sizeof * result_params * result_param_count);
    try(rset_current_row = malloc(result_length), NULL, fail);
    memset(rset_current_row, 0, result_length);
    try(timebased_result_params = malloc(sizeof * timebased_result_params * result_param_count), NULL, fail);
    memset(timebased_result_params, 0, sizeof * timebased_result_params * result_param_count);
    for (unsigned int i = 0; i < result_param_count; i++) {
        result_params[i].buffer_type = result_metadata->fields[i].type;
        if (result_params[i].buffer_type == MYSQL_TYPE_DATE || result_params[i].buffer_type == MYSQL_TYPE_TIME) {
            result_params[i].buffer = &(timebased_result_params[i]);
            result_params[i].buffer_length = sizeof(*timebased_result_params);
        }
        else {
            result_params[i].buffer = rset_current_row + result_bitmap[i + 1].offset;
            result_params[i].buffer_length = result_bitmap[i + 1].size;
        }
    }
    try((mysql_stmt_bind_result(statement, result_params) == 0), false, fail2);
    mysql_stmt_store_result(statement);
    rset_num_rows = mysql_stmt_num_rows(statement);
    try(*response = malloc(sizeof **response + result_length * rset_num_rows), NULL, fail2);
    memset(*response, 0, sizeof **response + result_length * rset_num_rows);
    result = (uint8_t*)*response + result_offest;
    (*response)->error_message = NULL;
    (*response)->num_elements = rset_num_rows;
    for (unsigned long long i = 0; i < rset_num_rows; i++) {
        try((mysql_stmt_fetch(statement) == 0), false, fail3);
        for (unsigned int j = 0; j < result_param_count; j++) {
            if (result_params[j].buffer_type == MYSQL_TYPE_DATE) {
                mysql_date_to_string(&(timebased_result_params[j]), rset_current_row + result_bitmap[j + 1].offset);
            }
            if (result_params[j].buffer_type == MYSQL_TYPE_TIME) {
                mysql_time_to_string(&(timebased_result_params[j]), rset_current_row + result_bitmap[j + 1].offset);
            }
        }
        memcpy(result + (result_length * i), rset_current_row, result_length);
    }
    while (mysql_stmt_next_result(statement) != -1);
    mysql_stmt_free_result(statement);
    mysql_stmt_reset(statement);
    return 0;
fail3:
    free(*response);
fail2:
    *response = malloc(sizeof * *response);
    asprintf(&((*response)->error_message), "%s", mysql_stmt_error(statement));
    mysql_stmt_free_result(statement);
    mysql_stmt_reset(statement);
    return 1;
fail:
    *response = malloc(sizeof * *response);
    asprintf(&((*response)->error_message), "%s", cms_get_error_message(cms));
    (*response)->num_elements = 0;
    return 1;
}

static int stmt_bind_param(struct statement_data statement_data, struct request_param* request_param) {
    unsigned long param_count;
    MYSQL_BIND* binding_params;
    MYSQL_TIME* timebased_params;
    param_count = mysql_stmt_param_count(statement_data.statement);
    try(binding_params = malloc(sizeof * binding_params * param_count), NULL, fail);
    memset(binding_params, 0, sizeof * binding_params * param_count);
    try(timebased_params = malloc(sizeof * timebased_params * param_count), NULL, fail);
    memset(timebased_params, 0, sizeof * timebased_params * param_count);
    for (unsigned long i = 0; i < param_count; i++) {
        binding_params[i].buffer_type = statement_data.params_type[i];
        if (binding_params[i].buffer_type == MYSQL_TYPE_DATE) {
            date_to_mysql_time(request_param[i].ptr, &(timebased_params[i]));
            binding_params[i].buffer = &(timebased_params[i]);
            binding_params[i].buffer_length = sizeof(*timebased_params);
        }
        else if (binding_params[i].buffer_type == MYSQL_TYPE_TIME) {
            time_to_mysql_time(request_param[i].ptr, &(timebased_params[i]));
            binding_params[i].buffer = &(timebased_params[i]);
            binding_params[i].buffer_length = sizeof(*timebased_params);
        }
        else {
            binding_params[i].buffer = request_param[i].ptr;
            binding_params[i].buffer_length = request_param[i].size;
        }
    }
    try((mysql_stmt_bind_param(statement_data.statement, binding_params) == 0), false, fail);
    return 0;
fail:
    free(binding_params);
    return 1;
}
