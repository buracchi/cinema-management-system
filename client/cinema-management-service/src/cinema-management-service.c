#include "cinema-management-service.h"

#include <mysql.h>
#include <buracchi/libcommon/containers/map/linked_list_map.h>
#include <buracchi/libcommon/containers/map.h>
#include <buracchi/libcommon/iterators/iterator.h>
#include <buracchi/libcommon/utilities/utilities.h>

#include "utilities/db.h"
#include "utilities/utilities.h"

struct cinema_management_service {
	MYSQL* db_connection;
	cmn_map_t mysql_statement_map;
};

static struct {
	const char* name;
	const char* template;
} statements[] = {
	{ "cancel_booking",								"call annulla_prenotazione(?)" },
	{ "assign_projectionist",						"call assegna_proiezionista(?, ?, ?, ?, ?)" },
	{ "book_seat",									"call effettua_prenotazione(?, ?, ?, ?, ?, ?, ?, ?, ?, ?)" },
	{ "delete_cinema",								"call elimina_cinema(?)" },
	{ "delete_employee",							"call elimina_dipendente(?)" },
	{ "delete_screening",							"call elimina_proiezione(?, ?, ?, ?)" },
	{ "delete_hall",								"call elimina_sala(?, ?)" },
	{ "delete_shift",								"call elimina_turno(?, ?, ?)" },
	{ "add_cinema",									"call inserisci_cinema(?, ?, ?)" },
	{ "add_employee",								"call inserisci_dipendente(?, ?, ?)" },
	{ "add_screening",								"call inserisci_proiezione(?, ?, ?, ?, ?, ?)" },
	{ "add_hall",									"call inserisci_sala(?, ?, ?, ?)" },
	{ "add_shift",									"call inserisci_turno(?, ?, ?, ?, ?)" },
	{ "get_all_cinema",								"call mostra_cinema()" },
	{ "report_cinema_without_ushers",				"call mostra_cinema_senza_maschere()" },
	{ "get_employees",								"call mostra_dipendenti()" },
	{ "get_cinema_screenings",						"call mostra_palinsesto(?)" },
	{ "get_avalilable_seats",						"call mostra_posti_disponibili(?, ?, ?, ?)" },
	{ "get_screenings",								"call mostra_proiezioni()" },
	{ "report_screenings_without_projectionist",	"call mostra_proiezioni_senza_proiezionista()" },
	{ "get_available_projectionists",				"call mostra_proiezionisti_disponibili(?, ?, ?, ?)" },
	{ "get_halls",									"call mostra_sale(?)" },
	{ "report_reservations_status",					"call mostra_stato_prenotazioni()" },
	{ "get_shifts",									"call mostra_turni()" },
	{ "validate_booking",							"call valida_prenotazione(?)" },
	{ NULL, NULL }
};

static bool initialize_prepared_stmts(void);
static int close_prepared_statements(void);
static errno_t connect(cinema_management_service_t service, const char* username, const char* password);

extern cinema_management_service_t cinema_management_service_init(const char* username, const char* password) {
	cinema_management_service_t this;
	try(this = malloc(sizeof * this), NULL, fail);
	try(this->mysql_statement_map = (cmn_map_t)cmn_linked_list_map_init(), NULL, fail2);
	cmn_map_set_key_comparer(this->mysql_statement_map, string_comparer);
	try(mysql_init(this->db_connection), NULL, fail3);
	connect(this, username, password);
	initialize_prepared_stmts(this);
	return this;
fail3:
	cmn_map_destroy(this->mysql_statement_map);
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
	mysql_real_connect(
		service->db_connection,
		getenv("HOST"),
		username,
		password,
		getenv("DB"),
		atoi(getenv("PORT")),
		NULL,
		CLIENT_MULTI_STATEMENTS | CLIENT_MULTI_RESULTS | CLIENT_COMPRESS | CLIENT_INTERACTIVE | CLIENT_REMEMBER_OPTIONS);
	mysql_options(service->db_connection, MYSQL_OPT_CONNECT_TIMEOUT, &timeout);
	mysql_options(service->db_connection, MYSQL_OPT_RECONNECT, &reconnect);
	return 0;
}

extern errno_t cinema_management_service_destroy(cinema_management_service_t service) {
	close_prepared_stmts(service);
	mysql_close(service->db_connection);
	cmn_map_destroy(service->mysql_statement_map);
	free(service);
	return 0;
}

static bool initialize_prepared_stmts(cinema_management_service_t service) {
	for (int i = 0; statements[i].name; i++) {
		static MYSQL_STMT* new_statement;
		if (!setup_prepared_stmt(&new_statement, statements[i].template, service->db_connection)) {
			char* message;
			asprintf(&message, "%s %s %s\n", "Unable to initialize", statements[i].name, "statement");
			return false;
		}
		cmn_map_insert(service->mysql_statement_map, statements[i].name, new_statement, NULL);
	}
	return true;
}

static int close_prepared_stmts(cinema_management_service_t service) {
	cmn_iterator_t iterator = cmn_map_begin(service->mysql_statement_map);
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
