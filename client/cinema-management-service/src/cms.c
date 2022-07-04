#include "cms/cms.h"

#include <assert.h>
#include <limits.h>
#include <mysql.h>
#include <buracchi/common/utilities/utilities.h>
#include <buracchi/common/utilities/try.h>

#include "cms_operation.h"
#include "utilities/dbutil.h"

#define FIELD_TYPE enum enum_field_types
#define MYSQL_USER_DEFINED_ERROR 1644
#define MYSQL_DUPLICATE_ENTRY_FOR_KEY_ERROR 1062
#define MYSQL_CANNOT_DELETE_OR_UPDATE_PARENT_ROW_FOREIGN_KEY_CONSTRAINT_FAILS 1451
#define MYSQL_CHECK_CONSTRAINTIS_VIOLATED_ERROR 3819

static const struct operation_data {
	MYSQL_STMT* statement;
	const char* const query;
	const FIELD_TYPE* const params_type;
} statements_read_only_data[OPERATIONS_NUMBER] = {
	[CANCEL_BOOKING] = {
		.statement = NULL,
		.query = "call annulla_prenotazione(?)",
		.params_type = (FIELD_TYPE[1]){FIELD_TYPE_STRING}
	},
	[ASSIGN_PROJECTIONIST] = {
		.statement = NULL,
		.query = "call assegna_proiezionista(?, ?, ?, ?, ?)",
		.params_type = (FIELD_TYPE[5]){FIELD_TYPE_LONG, FIELD_TYPE_LONG, FIELD_TYPE_LONG, FIELD_TYPE_DATE, FIELD_TYPE_TIME}
	},
	[DELETE_CINEMA] = {
		.statement = NULL,
		.query = "call elimina_cinema(?)",
		.params_type = (FIELD_TYPE[1]) { FIELD_TYPE_LONG }
	},
	[DELETE_EMPLOYEE] = {
		.statement = NULL,
		.query = "call elimina_dipendente(?)",
		.params_type = (FIELD_TYPE[1]) { FIELD_TYPE_LONG }
	},
	[ABORT_BOOKING] = {
		.statement = NULL,
		.query = "call elimina_prenotazione(?)",
		.params_type = (FIELD_TYPE[1]) { FIELD_TYPE_STRING }
	},
	[DELETE_PROJECTION] = {
		.statement = NULL,
		.query = "call elimina_proiezione(?, ?, ?, ?)",
		.params_type = (FIELD_TYPE[4]) { FIELD_TYPE_LONG, FIELD_TYPE_LONG, FIELD_TYPE_DATE, FIELD_TYPE_TIME }
	},
	[DELETE_HALL] = {
		.statement = NULL,
		.query = "call elimina_sala(?, ?)",
		.params_type = (FIELD_TYPE[2]) { FIELD_TYPE_LONG, FIELD_TYPE_LONG }
	},
	[DELETE_SHIFT] = {
		.statement = NULL,
		.query = "call elimina_turno(?, ?, ?)",
		.params_type = (FIELD_TYPE[3]) { FIELD_TYPE_LONG, FIELD_TYPE_STRING, FIELD_TYPE_TIME }
	},
	[COMMIT_BOOKING] = {
		.statement = NULL,
		.query = "call finalizza_prenotazione(?, ?, ?, ?, ?)",
		.params_type = (FIELD_TYPE[5]) { FIELD_TYPE_STRING, FIELD_TYPE_STRING, FIELD_TYPE_DECIMAL, FIELD_TYPE_DATE, FIELD_TYPE_DECIMAL }
	},
	[CREATE_BOOKING] = {
		.statement = NULL,
		.query = "call inizializza_prenotazione(?, ?, ?, ?, ?, ?)",
		.params_type = (FIELD_TYPE[6]) { FIELD_TYPE_LONG, FIELD_TYPE_LONG, FIELD_TYPE_DATE, FIELD_TYPE_TIME, FIELD_TYPE_STRING, FIELD_TYPE_LONG }
	},
	[ADD_CINEMA] = {
		.statement = NULL,
		.query = "call inserisci_cinema(?, ?, ?)",
		.params_type = (FIELD_TYPE[3]) { FIELD_TYPE_STRING, FIELD_TYPE_TIME, FIELD_TYPE_TIME }
	},
	[ADD_EMPLOYEE] = {
		.statement = NULL,
		.query = "call inserisci_dipendente(?, ?, ?)",
		.params_type = (FIELD_TYPE[3]) { FIELD_TYPE_STRING, FIELD_TYPE_STRING, FIELD_TYPE_STRING }
	},
	[ADD_PROJECTION] = {
		.statement = NULL,
		.query = "call inserisci_proiezione(?, ?, ?, ?, ?, ?)",
		.params_type = (FIELD_TYPE[6]) { FIELD_TYPE_LONG, FIELD_TYPE_LONG, FIELD_TYPE_DATE, FIELD_TYPE_TIME, FIELD_TYPE_DECIMAL, FIELD_TYPE_LONG }
	},
	[ADD_HALL] = {
		.statement = NULL,
		.query = "call inserisci_sala(?, ?, ?, ?)",
		.params_type = (FIELD_TYPE[4]) { FIELD_TYPE_LONG, FIELD_TYPE_LONG, FIELD_TYPE_LONG, FIELD_TYPE_LONG }
	},
	[ADD_SHIFT] = {
		.statement = NULL,
		.query = "call inserisci_turno(?, ?, ?, ?, ?)",
		.params_type = (FIELD_TYPE[5]) { FIELD_TYPE_LONG, FIELD_TYPE_STRING, FIELD_TYPE_TIME, FIELD_TYPE_TIME, FIELD_TYPE_LONG }
	},
	[GET_ALL_CINEMA] = {
		.statement = NULL,
		.query = "call mostra_cinema()",
		.params_type = NULL
	},
	[GET_CINEMA_WITHOUT_ENOUGH_USHERS] = {
		.statement = NULL,
		.query = "call mostra_cinema_senza_maschere()",
		.params_type = NULL
	},
	[GET_EMPLOYEES] = {
		.statement = NULL,
		.query = "call mostra_dipendenti()",
		.params_type = NULL
	},
	[GET_MOVIES] = {
		.statement = NULL,
		.query = "call mostra_film()",
		.params_type = NULL
	},
	[GET_CINEMA_SCREENINGS] = {
		.statement = NULL,
		.query = "call mostra_palinsesto(?)",
		.params_type = (FIELD_TYPE[1]) { FIELD_TYPE_LONG }
	},
	[GET_AVAILABLE_SEATS] = {
		.statement = NULL,
		.query = "call mostra_posti_disponibili(?, ?, ?, ?)",
		.params_type = (FIELD_TYPE[4]) { FIELD_TYPE_LONG, FIELD_TYPE_LONG, FIELD_TYPE_DATE, FIELD_TYPE_TIME }
	},
	[GET_PROJECTION] = {
		.statement = NULL,
		.query = "call mostra_proiezioni()",
		.params_type = NULL
	},
	[GET_SCREENINGS_WITHOUT_PROJECTIONIST] = {
		.statement = NULL,
		.query = "call mostra_proiezioni_senza_proiezionista()",
		.params_type = NULL
	},
	[GET_AVAILABLE_PROJECTIONISTS] = {
		.statement = NULL,
		.query = "call mostra_proiezionisti_disponibili(?, ?, ?, ?)",
		.params_type = (FIELD_TYPE[4]) { FIELD_TYPE_LONG, FIELD_TYPE_LONG, FIELD_TYPE_DATE, FIELD_TYPE_TIME }
	},
	[GET_HALLS] = {
		.statement = NULL,
		.query = "call mostra_sale(?)",
		.params_type = (FIELD_TYPE[1]) { FIELD_TYPE_LONG }
	},
	[GET_MONTHLY_RESERVATIONS_STATUS] = {
		.statement = NULL,
		.query = "call mostra_stato_prenotazioni()",
		.params_type = NULL
	},
	[GET_SHIFTS] = {
		.statement = NULL,
		.query = "call mostra_turni()",
		.params_type = NULL
	},
	[VALIDATE_BOOKING] = {
		.statement = NULL,
		.query = "call valida_prenotazione(?)",
		.params_type = (FIELD_TYPE[1]) { FIELD_TYPE_STRING }
	}
};

struct cms {
	MYSQL* db_connection;
	struct operation_data operation_data[OPERATIONS_NUMBER];
};

static MYSQL_STMT* get_prepared_stmt(cms_t cms, enum cms_operation operation, char** error_message);
static int send_mysql_stmt_request(struct operation_data operation_data, struct cms_request_param(*request_param)[]);
static int recv_mysql_stmt_result(struct operation_data operation_data, struct cms_response* response, struct cms_result_metadata* result_metdata);
static bool is_fatal_error(unsigned int mysql_errno);

extern cms_t cms_init(const struct cms_credentials* credentials) {
	cms_t this;
	unsigned int timeout = 30;
	bool reconnect = true;
	try(this = malloc(sizeof * this), NULL, fail);
	try(this->db_connection = mysql_init(NULL), NULL, fail2);
	mysql_options(this->db_connection, MYSQL_OPT_CONNECT_TIMEOUT, &timeout);
	mysql_options(this->db_connection, MYSQL_OPT_RECONNECT, &reconnect);
	try(mysql_real_connect(
		this->db_connection,
		credentials->host,
		credentials->username,
		credentials->password,
		credentials->db,
		credentials->port,
		NULL,
		CLIENT_MULTI_STATEMENTS | CLIENT_MULTI_RESULTS | CLIENT_COMPRESS | CLIENT_INTERACTIVE | CLIENT_REMEMBER_OPTIONS),
		NULL, fail3);
	memcpy(this->operation_data, statements_read_only_data, sizeof(statements_read_only_data));
	return this;
fail3:
	fprintf(stderr, "Error: %s\n", mysql_error(this->db_connection));
fail2:
	free(this);
fail:
	return NULL;
}

extern void cms_destroy(cms_t cms) {
	for (int i = 0; i < OPERATIONS_NUMBER; i++) {
		MYSQL_STMT* statement = cms->operation_data[i].statement;
		if (statement) {
			mysql_stmt_close(statement);
		}
	}
	mysql_close(cms->db_connection);
	mysql_library_end();
	free(cms);
}

extern inline void cms_destroy_response(struct cms_response* response) {
	free((void*)response->error_message);
	free(response->response_ptr);
}

extern inline const char* cms_get_error_message(cms_t cms) {
	return mysql_error(cms->db_connection);
}

extern struct cms_response cms_operation_execute(cms_t cms, enum cms_operation operation, struct cms_request_param(*request_param)[], struct cms_result_metadata* result_metdata) {
	struct cms_response response = {
			.fatal_error = false,
			.error_message = NULL,
			.num_elements = 0,
			.response_ptr = NULL
	};
	MYSQL_STMT* statement;
	try(statement = get_prepared_stmt(cms, operation, (char**)&(response.error_message)), NULL, fail);
	try(send_mysql_stmt_request(cms->operation_data[operation], request_param), 1, fail2);
	try(recv_mysql_stmt_result(cms->operation_data[operation], &response, result_metdata), 1, fail2);
	try(mysql_stmt_reset(statement) == 0, false, fail2);
	return response;
fail2:
	if (!response.error_message) {
		asprintf((char**)&(response.error_message), "%s", mysql_stmt_error(statement));
	}
	response.fatal_error = is_fatal_error(statement->last_errno);
	return response;
fail:
	asprintf((char**)&(response.error_message), "%s", cms_get_error_message(cms));
	response.fatal_error = true;
	return response;
}

static MYSQL_STMT* get_prepared_stmt(cms_t cms, enum cms_operation operation, char** error_message) {
	MYSQL_STMT** stmt = &(cms->operation_data[operation].statement);
	if (!*stmt) {
		const char* query = cms->operation_data[operation].query;
		bool update_length = true;
		try(*stmt = mysql_stmt_init(cms->db_connection), NULL, fail);
		try((mysql_stmt_prepare(*stmt, query, (unsigned long)strlen(query))) != 0, true, fail2);
		mysql_stmt_attr_set(*stmt, STMT_ATTR_UPDATE_MAX_LENGTH, &update_length);
	}
	return *stmt;
fail2:
	asprintf(error_message, "%s", mysql_stmt_error(*stmt));
	mysql_stmt_close(*stmt);
	*stmt = NULL;
fail:
	return *stmt;
}

static int send_mysql_stmt_request(struct operation_data operation_data, struct cms_request_param(*request_param)[]) {
	unsigned long param_count;
	MYSQL_BIND* bparams = NULL;
	MYSQL_TIME* tbparams = NULL;
	param_count = mysql_stmt_param_count(operation_data.statement);
	if (param_count) {
		assert(request_param != NULL && "Prepared statement requires parameters but no one was provided.");
		if (!request_param) {
			goto fail;
		}
		try(bparams = calloc(param_count, sizeof * bparams), NULL, fail);
		try(tbparams = calloc(param_count, sizeof * tbparams), NULL, fail2);
		for (unsigned long i = 0; i < param_count; i++) {
			(bparams)[i].buffer_type = operation_data.params_type[i];
			if ((bparams)[i].buffer_type == MYSQL_TYPE_DATE) {
				date_to_mysql_time((*request_param)[i].ptr, &(tbparams[i]));
				(bparams)[i].buffer = &(tbparams[i]);
				(bparams)[i].buffer_length = sizeof(*tbparams);
			}
			else if ((bparams)[i].buffer_type == MYSQL_TYPE_TIME) {
				time_to_mysql_time((*request_param)[i].ptr, &(tbparams[i]));
				(bparams)[i].buffer = &(tbparams[i]);
				(bparams)[i].buffer_length = sizeof(*tbparams);
			}
			else {
				(bparams)[i].buffer = (*request_param)[i].ptr;
				assert((*request_param)[i].size <= ULONG_MAX && "MYSQL C API doesn't support the size of the type chosen");
				if ((*request_param)[i].size > ULONG_MAX) goto fail3; // If the application was debugged this should be dead code
				(bparams)[i].buffer_length = (unsigned long)(*request_param)[i].size;
				if ((bparams)[i].buffer_type == MYSQL_TYPE_STRING) {
					(bparams)[i].buffer_length--;
				}
			}
		}
		try((mysql_stmt_bind_param(operation_data.statement, bparams) == 0), false, fail3);
	}
	try((mysql_stmt_execute(operation_data.statement) == 0), false, fail3);
	free(bparams);
	free(tbparams);
	return 0;
fail3:
	free(tbparams);
fail2:
	free(bparams);
fail:
	return 1;
}

static int recv_mysql_stmt_result(struct operation_data operation_data, struct cms_response* response, struct cms_result_metadata* result_metdata) {
	unsigned int rparam_count;
	MYSQL_TIME* trparams;
	MYSQL_BIND* rparams;
	uint8_t* rset_current_row_buffer;
	rparam_count = operation_data.statement->field_count;
	if (!rparam_count) {
		response->num_elements = mysql_stmt_affected_rows(operation_data.statement);
		return 0;
	}
	if (!result_metdata->offset_size_bitmap) {
		asprintf((char**)&(response->error_message), "Received result set of %u fields but no fields were expected.", rparam_count);
		response->fatal_error = true;
		return 0;
	}
	try(rparams = calloc(rparam_count, sizeof * rparams), NULL, out_of_memory);
	try(trparams = calloc(rparam_count, sizeof * trparams), NULL, out_of_memory2);
	try(rset_current_row_buffer = calloc(1, result_metdata->size), NULL, out_of_memory3);
	for (unsigned int i = 0; i < rparam_count; i++) {
		rparams[i].buffer_type = operation_data.statement->fields[i].type;
		if (rparams[i].buffer_type == MYSQL_TYPE_DATE || rparams[i].buffer_type == MYSQL_TYPE_TIME) {
			rparams[i].buffer = &(trparams[i]);
			rparams[i].buffer_length = sizeof(*trparams);
		}
		else {
			rparams[i].buffer = rset_current_row_buffer + result_metdata->offset_size_bitmap[i].offset;
			assert(result_metdata->offset_size_bitmap[i].size <= ULONG_MAX && "MYSQL C API doesn't support the size of the type chosen");
			if (result_metdata->offset_size_bitmap[i].size > ULONG_MAX) goto fail; // If the application was debugged this should be dead code
			rparams[i].buffer_length = (unsigned long)result_metdata->offset_size_bitmap[i].size;
		}
	}
	try(mysql_stmt_bind_result(operation_data.statement, rparams) == 0, false, fail);
	for (unsigned int i = 0; i < rparam_count; i++) {
		unsigned long required_length = operation_data.statement->bind[i].length_value;
		unsigned long buffer_length = operation_data.statement->bind[i].buffer_length;
		if (operation_data.statement->bind[i].buffer_type != MYSQL_TYPE_VAR_STRING
			&& operation_data.statement->bind[i].buffer_type != MYSQL_TYPE_STRING
			&& operation_data.statement->bind[i].buffer_type != MYSQL_TYPE_NEWDECIMAL) {
			assert(required_length == buffer_length
				&& "The type representation chosen for a result field has a byte size \
different from the one required by the server,\
MySQL C API knows it but it is so bad that will corrupt the memory instead of fail");
		}
	}
	try(mysql_stmt_store_result(operation_data.statement) == 0, false, fail);
	response->num_elements = mysql_stmt_num_rows(operation_data.statement);
	try(*result_metdata->ptr = malloc(result_metdata->size * response->num_elements), NULL, out_of_memory4);
	response->response_ptr = *result_metdata->ptr;
	for (uint64_t i = 0; i < response->num_elements; i++) {
		try(mysql_stmt_fetch(operation_data.statement) == 0, false, fail2);
		for (unsigned int j = 0; j < rparam_count; j++) {
			if (rparams[j].buffer_type == MYSQL_TYPE_DATE) {
				mysql_date_to_string(&(trparams[j]), (char*)(rset_current_row_buffer + result_metdata->offset_size_bitmap[j].offset));
			}
			if (rparams[j].buffer_type == MYSQL_TYPE_TIME) {
				mysql_time_to_string(&(trparams[j]), (char*)(rset_current_row_buffer + result_metdata->offset_size_bitmap[j].offset));
			}
		}
		memcpy(*((uint8_t**)result_metdata->ptr) + (result_metdata->size * i), rset_current_row_buffer, result_metdata->size);
		memset(rset_current_row_buffer, 0, result_metdata->size);
	}
	int ret = 0;
	while (ret != -1) {
		try((ret = mysql_stmt_next_result(operation_data.statement)) < 1, false, fail2);
	}
	try(mysql_stmt_free_result(operation_data.statement) == 0, false, fail);
	free(rparams);
	free(trparams);
	free(rset_current_row_buffer);
	return 0;
fail2:
	mysql_stmt_free_result(operation_data.statement);
fail:
	free(rset_current_row_buffer);
	free(trparams);
	free(rparams);
	return 1;
out_of_memory4:
	mysql_stmt_free_result(operation_data.statement);
out_of_memory3:
	free(trparams);
out_of_memory2:
	free(rparams);
out_of_memory:
	asprintf((char**)&(response->error_message),
		"There is not enough memory to complete this operation. Try closing other application to free more memory.");
	response->fatal_error = true;
	return 0;
}

//TODO: Complete the fallthrough list of non fatal errors
static bool is_fatal_error(unsigned int mysql_errno) {
	switch (mysql_errno) {
	case MYSQL_USER_DEFINED_ERROR:
	case MYSQL_DUPLICATE_ENTRY_FOR_KEY_ERROR:
	case MYSQL_CANNOT_DELETE_OR_UPDATE_PARENT_ROW_FOREIGN_KEY_CONSTRAINT_FAILS:
	case MYSQL_CHECK_CONSTRAINTIS_VIOLATED_ERROR:
		return false;
	default:
		return true;
	}
}
