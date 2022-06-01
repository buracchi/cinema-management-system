#pragma once

#include <stdint.h>
#include <mysql.h>

#include "cms.h"

#define FIELD_TYPE enum enum_field_types

struct request_param {
	void* ptr;
	size_t size;
};

struct result_bitmap {
	size_t offset;
	size_t size;
};

enum statement_operation {
	ADD_CINEMA,
	ADD_EMPLOYEE,
	ADD_HALL,
	ADD_SCREENING,
	ADD_SHIFT,
	ASSIGN_PROJECTIONIST,
	BOOK_SEAT,
	CANCEL_BOOKING,
	DELETE_CINEMA,
	DELETE_EMPLOYEE,
	DELETE_HALL,
	DELETE_SCREENING,
	DELETE_SHIFT,
	GET_ALL_CINEMA,
	GET_AVAILABLE_PROJECTIONISTS,
	GET_AVALILABLE_SEATS,
	GET_CINEMA_SCREENINGS,
	GET_EMPLOYEES,
	GET_HALLS,
	GET_SCREENINGS,
	GET_SHIFTS,
	REPORT_CINEMA_WITHOUT_USHERS,
	REPORT_RESERVATIONS_STATUS,
	REPORT_SCREENINGS_WITHOUT_PROJECTIONIST,
	VALIDATE_BOOKING,
	PREPARED_STATEMENT_NUMBER
};

static const struct statement_data {
	MYSQL_STMT* statement;
	const char* const query;
	const FIELD_TYPE* const params_type;
} statements_data[PREPARED_STATEMENT_NUMBER] = {
	[CANCEL_BOOKING] = {
		.statement = NULL,
		.query = "call annulla_prenotazione(?)",
		.params_type = (FIELD_TYPE[1]){FIELD_TYPE_LONG}
	},
	[ASSIGN_PROJECTIONIST] = {
		.statement = NULL,
		.query = "call assegna_proiezionista(?, ?, ?, ?, ?)",
		.params_type = (FIELD_TYPE[5]){FIELD_TYPE_LONG, FIELD_TYPE_LONG, FIELD_TYPE_LONG, FIELD_TYPE_DATE, FIELD_TYPE_TIME}
	},
	[BOOK_SEAT] = {
		.statement = NULL,
		.query = "call effettua_prenotazione(?, ?, ?, ?, ?, ?, ?, ?, ?, ?)",
		.params_type = (FIELD_TYPE[10]) { FIELD_TYPE_LONG, FIELD_TYPE_LONG, FIELD_TYPE_DATE, FIELD_TYPE_TIME, FIELD_TYPE_STRING, FIELD_TYPE_LONG, FIELD_TYPE_STRING, FIELD_TYPE_DECIMAL, FIELD_TYPE_DATE, FIELD_TYPE_DECIMAL }
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
	[DELETE_SCREENING] = {
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
	[ADD_SCREENING] = {
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
	[REPORT_CINEMA_WITHOUT_USHERS] = {
		.statement = NULL,
		.query = "call mostra_cinema_senza_maschere()",
		.params_type = NULL
	},
	[GET_EMPLOYEES] = {
		.statement = NULL,
		.query = "call mostra_dipendenti()",
		.params_type = NULL
	},
	[GET_CINEMA_SCREENINGS] = {
		.statement = NULL,
		.query = "call mostra_palinsesto(?)",
		.params_type = (FIELD_TYPE[1]) { FIELD_TYPE_LONG }
	},
	[GET_AVALILABLE_SEATS] = {
		.statement = NULL,
		.query = "call mostra_posti_disponibili(?, ?, ?, ?)",
		.params_type = (FIELD_TYPE[4]) { FIELD_TYPE_LONG, FIELD_TYPE_LONG, FIELD_TYPE_DATE, FIELD_TYPE_TIME }
	},
	[GET_SCREENINGS] = {
		.statement = NULL,
		.query = "call mostra_proiezioni()",
		.params_type = NULL
	},
	[REPORT_SCREENINGS_WITHOUT_PROJECTIONIST] = {
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
	[REPORT_RESERVATIONS_STATUS] = {
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
		.params_type = (FIELD_TYPE[1]) { FIELD_TYPE_LONG }
	}
};

extern MYSQL_STMT* get_prepared_stmt(cms_t cms, enum statement_operation operation);

extern int cms_stmt_execute(cms_t, enum statement_operation, struct request_param*, struct cms_result_response**, struct result_bitmap*);
