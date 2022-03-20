#include "cinema-management-service.h"

#include <mysql.h>
#include <buracchi/common/containers/map/linked_list_map.h>
#include <buracchi/common/utilities/utilities.h>
#include <buracchi/common/utilities/try.h>

#include "type/cinema-management-service.h"
#include "utilities/dbutil.h"
#include "utilities/utilities.h"

static struct {
	const char* name;
	const char* template;
	uint8_t required_params;
	FIELD_TYPE* params_type;
	uint8_t result_columns;
	FIELD_TYPE* result_columns_type;
} statements[] = {
	{ 
		.name = "cancel_booking",
		.template = "call annulla_prenotazione(?)",
		.required_params = 1,
		.params_type = (FIELD_TYPE[1]) { FIELD_TYPE_LONG },
		.result_columns = 0,
		.result_columns_type = NULL
	},
	{
		.name = "assign_projectionist",
		.template = "call assegna_proiezionista(?, ?, ?, ?, ?)",
		.required_params = 5,
		.params_type = (FIELD_TYPE[5]) { FIELD_TYPE_LONG, FIELD_TYPE_LONG, FIELD_TYPE_LONG, FIELD_TYPE_DATE, FIELD_TYPE_TIME },
		.result_columns = 0,
		.result_columns_type = NULL
	},
	{
		.name = "book_seat",
		.template = "call effettua_prenotazione(?, ?, ?, ?, ?, ?, ?, ?, ?, ?)",
		.required_params = 10,
		.params_type = (FIELD_TYPE[10]) { FIELD_TYPE_LONG, FIELD_TYPE_LONG, FIELD_TYPE_DATE, FIELD_TYPE_TIME, FIELD_TYPE_STRING, FIELD_TYPE_LONG, FIELD_TYPE_STRING, FIELD_TYPE_DECIMAL, FIELD_TYPE_DATE, FIELD_TYPE_DECIMAL },
		.result_columns = 1,
		.result_columns_type = (FIELD_TYPE[1]) { FIELD_TYPE_LONG }
	},
	{
		.name = "delete_cinema",
		.template = "call elimina_cinema(?)",
		.required_params = 1,
		.params_type = (FIELD_TYPE[1]) { FIELD_TYPE_LONG },
		.result_columns = 0,
		.result_columns_type = NULL
	},
	{
		.name = "delete_employee",
		.template = "call elimina_dipendente(?)",
		.required_params = 1,
		.params_type = (FIELD_TYPE[1]) { FIELD_TYPE_LONG },
		.result_columns = 0,
		.result_columns_type = NULL
	},
	{
		.name = "delete_screening",
		.template = "call elimina_proiezione(?, ?, ?, ?)",
		.required_params = 4,
		.params_type = (FIELD_TYPE[4]) { FIELD_TYPE_LONG, FIELD_TYPE_LONG, FIELD_TYPE_DATE, FIELD_TYPE_TIME },
		.result_columns = 0,
		.result_columns_type = NULL
	},
	{
		.name = "delete_hall",
		.template = "call elimina_sala(?, ?)",
		.required_params = 2,
		.params_type = (FIELD_TYPE[2]) { FIELD_TYPE_LONG, FIELD_TYPE_LONG },
		.result_columns = 0,
		.result_columns_type = NULL
	},
	{
		.name = "delete_shift",
		.template = "call elimina_turno(?, ?, ?)",
		.required_params = 3,
		.params_type = (FIELD_TYPE[3]) { FIELD_TYPE_LONG, FIELD_TYPE_STRING, FIELD_TYPE_TIME },
		.result_columns = 0,
		.result_columns_type = NULL
	},
	{
		.name = "add_cinema",
		.template = "call inserisci_cinema(?, ?, ?)",
		.required_params = 3,
		.params_type = (FIELD_TYPE[3]) { FIELD_TYPE_STRING, FIELD_TYPE_TIME, FIELD_TYPE_TIME },
		.result_columns = 0,
		.result_columns_type = NULL
	},
	{
		.name = "add_employee",
		.template = "call inserisci_dipendente(?, ?, ?)",
		.required_params = 3,
		.params_type = (FIELD_TYPE[3]) { FIELD_TYPE_STRING, FIELD_TYPE_STRING, FIELD_TYPE_STRING },
		.result_columns = 0,
		.result_columns_type = NULL
	},
	{
		.name = "add_screening",
		.template = "call inserisci_proiezione(?, ?, ?, ?, ?, ?)",
		.required_params = 6,
		.params_type = (FIELD_TYPE[6]) { FIELD_TYPE_LONG, FIELD_TYPE_LONG, FIELD_TYPE_DATE, FIELD_TYPE_TIME, FIELD_TYPE_DECIMAL, FIELD_TYPE_LONG },
		.result_columns = 0,
		.result_columns_type = NULL
	},
	{
		.name = "add_hall",
		.template = "call inserisci_sala(?, ?, ?, ?)",
		.required_params = 4,
		.params_type = (FIELD_TYPE[4]) { FIELD_TYPE_LONG, FIELD_TYPE_LONG, FIELD_TYPE_LONG, FIELD_TYPE_LONG },
		.result_columns = 0,
		.result_columns_type = NULL
	},
	{
		.name = "add_shift",
		.template = "call inserisci_turno(?, ?, ?, ?, ?)",
		.required_params = 5,
		.params_type = (FIELD_TYPE[5]) { FIELD_TYPE_LONG, FIELD_TYPE_STRING, FIELD_TYPE_TIME, FIELD_TYPE_TIME, FIELD_TYPE_LONG },
		.result_columns = 0,
		.result_columns_type = NULL
	},
	{
		.name = "get_all_cinema",
		.template = "call mostra_cinema()",
		.required_params = 0,
		.params_type = NULL,
		.result_columns = 4,
		.result_columns_type = (FIELD_TYPE[4]) { FIELD_TYPE_LONG, FIELD_TYPE_STRING, FIELD_TYPE_TIME, FIELD_TYPE_TIME }
	},
	{
		.name = "report_cinema_without_ushers",
		.template = "call mostra_cinema_senza_maschere()",
		.required_params = 0,
		.params_type = NULL,
		.result_columns = 5,
		.result_columns_type = (FIELD_TYPE[5]) { FIELD_TYPE_LONG, FIELD_TYPE_STRING, FIELD_TYPE_TIME, FIELD_TYPE_TIME, FIELD_TYPE_LONG }
	},
	{
		.name = "get_employees",
		.template = "call mostra_dipendenti()",
		.required_params = 0,
		.params_type = NULL,
		.result_columns = 4,
		.result_columns_type = (FIELD_TYPE[4]) { FIELD_TYPE_LONG, FIELD_TYPE_STRING, FIELD_TYPE_STRING, FIELD_TYPE_STRING }
	},
	{
		.name = "get_cinema_screenings",
		.template = "call mostra_palinsesto(?)",
		.required_params = 1,
		.params_type = (FIELD_TYPE[1]) { FIELD_TYPE_LONG },
		.result_columns = 10,
		.result_columns_type = (FIELD_TYPE[10]) { FIELD_TYPE_DATE, FIELD_TYPE_TIME, FIELD_TYPE_LONG, FIELD_TYPE_LONG, FIELD_TYPE_DECIMAL, FIELD_TYPE_LONG, FIELD_TYPE_STRING, FIELD_TYPE_TIME, FIELD_TYPE_STRING, FIELD_TYPE_STRING }
	},
	{
		.name = "get_avalilable_seats",
		.template = "call mostra_posti_disponibili(?, ?, ?, ?)",
		.required_params = 4,
		.params_type = (FIELD_TYPE[4]) { FIELD_TYPE_LONG, FIELD_TYPE_LONG, FIELD_TYPE_DATE, FIELD_TYPE_TIME },
		.result_columns = 2,
		.result_columns_type = (FIELD_TYPE[2]) { FIELD_TYPE_STRING, FIELD_TYPE_LONG }
	},
	{
		.name = "get_screenings",
		.template = "call mostra_proiezioni()",
		.required_params = 0,
		.params_type = NULL,
		.result_columns = 9,
		.result_columns_type = (FIELD_TYPE[9]) { FIELD_TYPE_DATE, FIELD_TYPE_LONG, FIELD_TYPE_LONG, FIELD_TYPE_LONG, FIELD_TYPE_STRING, FIELD_TYPE_DECIMAL, FIELD_TYPE_TIME, FIELD_TYPE_TIME, FIELD_TYPE_LONG }
	},
	{
		.name = "report_screenings_without_projectionist",
		.template = "call mostra_proiezioni_senza_proiezionista()",
		.required_params = 0,
		.params_type = NULL,
		.result_columns = 7,
		.result_columns_type = (FIELD_TYPE[7]) { FIELD_TYPE_LONG, FIELD_TYPE_LONG, FIELD_TYPE_DATE, FIELD_TYPE_TIME, FIELD_TYPE_DECIMAL, FIELD_TYPE_LONG, FIELD_TYPE_LONG }
	},
	{
		.name = "get_available_projectionists",
		.template = "call mostra_proiezionisti_disponibili(?, ?, ?, ?)",
		.required_params = 4,
		.params_type = (FIELD_TYPE[4]) { FIELD_TYPE_LONG, FIELD_TYPE_LONG, FIELD_TYPE_DATE, FIELD_TYPE_TIME },
		.result_columns = 3,
		.result_columns_type = (FIELD_TYPE[3]) { FIELD_TYPE_LONG, FIELD_TYPE_STRING, FIELD_TYPE_STRING }
	},
	{
		.name = "get_halls",
		.template = "call mostra_sale(?)",
		.required_params = 1,
		.params_type = (FIELD_TYPE[1]) { FIELD_TYPE_LONG },
		.result_columns = 4,
		.result_columns_type = (FIELD_TYPE[4]) { FIELD_TYPE_LONG, FIELD_TYPE_LONG, FIELD_TYPE_LONG, FIELD_TYPE_LONG }
	},
	{
		.name = "report_reservations_status",
		.template = "call mostra_stato_prenotazioni()",
		.required_params = 0,
		.params_type = NULL,
		.result_columns = 5,
		.result_columns_type = (FIELD_TYPE[5]) { FIELD_TYPE_LONG, FIELD_TYPE_LONG, FIELD_TYPE_LONG, FIELD_TYPE_LONG, FIELD_TYPE_LONG }
	},
	{
		.name = "get_shifts",
		.template = "call mostra_turni()",
		.required_params = 0,
		.params_type = NULL,
		.result_columns = 8,
		.result_columns_type = (FIELD_TYPE[8]) { FIELD_TYPE_DATE, FIELD_TYPE_TIME, FIELD_TYPE_TIME, FIELD_TYPE_LONG, FIELD_TYPE_LONG, FIELD_TYPE_STRING, FIELD_TYPE_STRING, FIELD_TYPE_STRING }
	},
	{
		.name = "validate_booking",
		.template = "call valida_prenotazione(?)",
		.required_params = 1,
		.params_type = (FIELD_TYPE[1]) { FIELD_TYPE_LONG },
		.result_columns = 0,
		.result_columns_type = NULL
	},
	{
		.name = NULL,
		.template = NULL,
		.required_params = 1,
		.params_type = NULL,
		.result_columns = 0,
		.result_columns_type = NULL
	}
};

static bool initialize_prepared_stmts(cinema_management_service_t service);
static int close_prepared_statements(cinema_management_service_t service);
static errno_t connect(cinema_management_service_t service, const char* username, const char* password);

extern cinema_management_service_t cinema_management_service_init(const char* username, const char* password) {
	cinema_management_service_t this;
	try(this = malloc(sizeof * this), NULL, fail);
	try(this->mysql_statement_data_map = (cmn_map_t) cmn_linked_list_map_init(), NULL, fail2);
	cmn_map_set_key_comparer(this->mysql_statement_data_map, string_comparer);
	try(this->db_connection = mysql_init(NULL), NULL, fail3);
	connect(this, username, password);
	initialize_prepared_stmts(this);
	return this;
fail3:
	cmn_map_destroy(this->mysql_statement_data_map);
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
	close_prepared_stmts(service);
	mysql_close(service->db_connection);
	cmn_map_destroy(service->mysql_statement_data_map);
	free(service);
	return 0;
}

static bool initialize_prepared_stmts(cinema_management_service_t service) {
	for (int i = 0; statements[i].name; i++) {
		struct mysql_statement_data * statement_data;
		try(statement_data = malloc(sizeof * statement_data), NULL, fail);
		statement_data->param_types = statements[i].params_type;
		statement_data->required_params = statements[i].required_params;
		statement_data->result_columns = statements[i].result_columns;
		statement_data->result_columns_type = statements[i].result_columns_type;
		if (!setup_prepared_stmt(&(statement_data->statement), statements[i].template, service->db_connection)) {
			if (mysql_errno(service->db_connection) == 1370) {
				continue;
			}
			char* message;
			asprintf(&message, "%s %s %s : %s\n", "Unable to initialize", statements[i].name, "statement", mysql_error(service->db_connection));
			free(statement_data);
			return false;
		}
		cmn_map_insert(service->mysql_statement_data_map, (void*) statements[i].name, statement_data, NULL);
	}
	return true;
fail:
	return false;
}

static int close_prepared_stmts(cinema_management_service_t service) {
	cmn_iterator_t iterator = cmn_map_begin(service->mysql_statement_data_map);
	while (!cmn_iterator_end(iterator)) {
		MYSQL_STMT* statement = cmn_iterator_data(iterator);
		if (statement) {
			mysql_stmt_close(statement);
		}
		cmn_iterator_next(iterator);
	}
	cmn_iterator_destroy(iterator);
	return 0;
}
