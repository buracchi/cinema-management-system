#include "cinema-management-service.h"

#include <mysql.h>
#include <buracchi/common/containers/map/linked_list_map.h>
#include <buracchi/common/utilities/utilities.h>
#include <buracchi/common/utilities/try.h>

#include "utilities/db.h"
#include "utilities/utilities.h"

#define FIELD_TYPE enum enum_field_types

struct cinema_management_service {
	MYSQL* db_connection;
	cmn_map_t mysql_statement_data_map;
};

struct mysql_statement_data {
	MYSQL_STMT* statement;
	FIELD_TYPE* param_types;
};

/*
* FIELD_TYPE_NULL or MYSQL_TYPE_NULL are used as list terminator,
* do not use it for data values * always NULL, just pick the right data type.
*/
static struct {
	const char* name;
	const char* template;
	FIELD_TYPE* param_types;
} statements[] = {
	{ "cancel_booking",								"call annulla_prenotazione(?)",									(FIELD_TYPE[2]) { FIELD_TYPE_LONG, FIELD_TYPE_NULL }},
	{ "assign_projectionist",						"call assegna_proiezionista(?, ?, ?, ?, ?)",					(FIELD_TYPE[6]) { FIELD_TYPE_LONG, FIELD_TYPE_LONG, FIELD_TYPE_LONG, FIELD_TYPE_DATE, FIELD_TYPE_TIME, FIELD_TYPE_NULL } },
	{ "book_seat",									"call effettua_prenotazione(?, ?, ?, ?, ?, ?, ?, ?, ?, ?)",		(FIELD_TYPE[11]){ FIELD_TYPE_LONG, FIELD_TYPE_LONG, FIELD_TYPE_DATE, FIELD_TYPE_TIME, FIELD_TYPE_STRING, FIELD_TYPE_LONG, FIELD_TYPE_STRING, FIELD_TYPE_NEWDECIMAL, FIELD_TYPE_DATE, FIELD_TYPE_NEWDECIMAL, FIELD_TYPE_NULL } },
	{ "delete_cinema",								"call elimina_cinema(?)",										(FIELD_TYPE[2]) { FIELD_TYPE_LONG, FIELD_TYPE_NULL } },
	{ "delete_employee",							"call elimina_dipendente(?)",									(FIELD_TYPE[2]) { FIELD_TYPE_LONG, FIELD_TYPE_NULL } },
	{ "delete_screening",							"call elimina_proiezione(?, ?, ?, ?)",							(FIELD_TYPE[5]) { FIELD_TYPE_LONG, FIELD_TYPE_LONG, FIELD_TYPE_DATE, FIELD_TYPE_TIME, FIELD_TYPE_NULL } },
	{ "delete_hall",								"call elimina_sala(?, ?)",										(FIELD_TYPE[3]) { FIELD_TYPE_LONG, FIELD_TYPE_LONG, FIELD_TYPE_NULL } },
	{ "delete_shift",								"call elimina_turno(?, ?, ?)",									(FIELD_TYPE[4]) { FIELD_TYPE_LONG, FIELD_TYPE_STRING, FIELD_TYPE_TIME, FIELD_TYPE_NULL } },
	{ "add_cinema",									"call inserisci_cinema(?, ?, ?)",								(FIELD_TYPE[4]) { FIELD_TYPE_STRING, FIELD_TYPE_TIME, FIELD_TYPE_TIME, FIELD_TYPE_NULL } },
	{ "add_employee",								"call inserisci_dipendente(?, ?, ?)",							(FIELD_TYPE[4]) { FIELD_TYPE_STRING, FIELD_TYPE_STRING, FIELD_TYPE_STRING, FIELD_TYPE_NULL } },
	{ "add_screening",								"call inserisci_proiezione(?, ?, ?, ?, ?, ?)",					(FIELD_TYPE[7]) { FIELD_TYPE_LONG, FIELD_TYPE_LONG, FIELD_TYPE_DATE, FIELD_TYPE_TIME, FIELD_TYPE_NEWDECIMAL, FIELD_TYPE_LONG, FIELD_TYPE_NULL } },
	{ "add_hall",									"call inserisci_sala(?, ?, ?, ?)",								(FIELD_TYPE[5]) { FIELD_TYPE_LONG, FIELD_TYPE_LONG, FIELD_TYPE_LONG, FIELD_TYPE_LONG, FIELD_TYPE_NULL } },
	{ "add_shift",									"call inserisci_turno(?, ?, ?, ?, ?)",							(FIELD_TYPE[6]) { FIELD_TYPE_LONG, FIELD_TYPE_STRING, FIELD_TYPE_TIME, FIELD_TYPE_TIME, FIELD_TYPE_LONG, FIELD_TYPE_NULL } },
	{ "get_all_cinema",								"call mostra_cinema()",											(FIELD_TYPE[1]) { FIELD_TYPE_NULL } },
	{ "report_cinema_without_ushers",				"call mostra_cinema_senza_maschere()",							(FIELD_TYPE[1]) { FIELD_TYPE_NULL } },
	{ "get_employees",								"call mostra_dipendenti()",										(FIELD_TYPE[1]) { FIELD_TYPE_NULL } },
	{ "get_cinema_screenings",						"call mostra_palinsesto(?)",									(FIELD_TYPE[2]) { FIELD_TYPE_LONG, FIELD_TYPE_NULL } },
	{ "get_avalilable_seats",						"call mostra_posti_disponibili(?, ?, ?, ?)",					(FIELD_TYPE[5]) { FIELD_TYPE_LONG, FIELD_TYPE_LONG, FIELD_TYPE_DATE, FIELD_TYPE_TIME, FIELD_TYPE_NULL } },
	{ "get_screenings",								"call mostra_proiezioni()",										(FIELD_TYPE[1]) { FIELD_TYPE_NULL } },
	{ "report_screenings_without_projectionist",	"call mostra_proiezioni_senza_proiezionista()",					(FIELD_TYPE[1]) { FIELD_TYPE_NULL } },
	{ "get_available_projectionists",				"call mostra_proiezionisti_disponibili(?, ?, ?, ?)",			(FIELD_TYPE[5]) { FIELD_TYPE_LONG, FIELD_TYPE_LONG, FIELD_TYPE_DATE, FIELD_TYPE_TIME, FIELD_TYPE_NULL } },
	{ "get_halls",									"call mostra_sale(?)",											(FIELD_TYPE[2]) { FIELD_TYPE_LONG, FIELD_TYPE_NULL } },
	{ "report_reservations_status",					"call mostra_stato_prenotazioni()",								(FIELD_TYPE[1]) { FIELD_TYPE_NULL } },
	{ "get_shifts",									"call mostra_turni()",											(FIELD_TYPE[1]) { FIELD_TYPE_NULL } },
	{ "validate_booking",							"call valida_prenotazione(?)",									(FIELD_TYPE[2]) { FIELD_TYPE_LONG, FIELD_TYPE_NULL } },
	{ NULL,											NULL,															NULL }
};

static bool initialize_prepared_stmts(cinema_management_service_t service);
static int close_prepared_statements(cinema_management_service_t service);
static errno_t connect(cinema_management_service_t service, const char* username, const char* password);

extern cinema_management_service_t cinema_management_service_init(const char* username, const char* password) {
	cinema_management_service_t this;
	try(this = malloc(sizeof * this), NULL, fail);
	try(this->mysql_statement_data_map = (cmn_map_t) cmn_linked_list_map_init(), NULL, fail2);
	cmn_map_set_key_comparer(this->mysql_statement_data_map, string_comparer);
	try(mysql_init(this->db_connection), NULL, fail3);
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
		statement_data->param_types = statements[i].param_types;
		if (!setup_prepared_stmt(&(statement_data->statement), statements[i].template, service->db_connection)) {
			char* message;
			asprintf(&message, "%s %s %s\n", "Unable to initialize", statements[i].name, "statement");
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
