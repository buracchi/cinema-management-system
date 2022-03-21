#pragma once

#include <stdint.h>
#include <mysql.h>

#define FIELD_TYPE enum enum_field_types

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
	const uint8_t required_params;
	const FIELD_TYPE* const params_type;
	const uint8_t result_columns;
	const FIELD_TYPE* const result_columns_type;
} statements_data[PREPARED_STATEMENT_NUMBER] = {
	[CANCEL_BOOKING] = {
		.statement = NULL,
		.query = "call annulla_prenotazione(?)",
		.required_params = 1,
		.params_type = (FIELD_TYPE[1]) { FIELD_TYPE_LONG },
		.result_columns = 0,
		.result_columns_type = NULL
	},
	[ASSIGN_PROJECTIONIST] = {
		.statement = NULL,
		.query = "call assegna_proiezionista(?, ?, ?, ?, ?)",
		.required_params = 5,
		.params_type = (FIELD_TYPE[5]) { FIELD_TYPE_LONG, FIELD_TYPE_LONG, FIELD_TYPE_LONG, FIELD_TYPE_DATE, FIELD_TYPE_TIME },
		.result_columns = 0,
		.result_columns_type = NULL
	},
	[BOOK_SEAT] = {
		.statement = NULL,
		.query = "call effettua_prenotazione(?, ?, ?, ?, ?, ?, ?, ?, ?, ?)",
		.required_params = 10,
		.params_type = (FIELD_TYPE[10]) { FIELD_TYPE_LONG, FIELD_TYPE_LONG, FIELD_TYPE_DATE, FIELD_TYPE_TIME, FIELD_TYPE_STRING, FIELD_TYPE_LONG, FIELD_TYPE_STRING, FIELD_TYPE_DECIMAL, FIELD_TYPE_DATE, FIELD_TYPE_DECIMAL },
		.result_columns = 1,
		.result_columns_type = (FIELD_TYPE[1]) { FIELD_TYPE_LONG }
	},
	[DELETE_CINEMA] = {
		.statement = NULL,
		.query = "call elimina_cinema(?)",
		.required_params = 1,
		.params_type = (FIELD_TYPE[1]) { FIELD_TYPE_LONG },
		.result_columns = 0,
		.result_columns_type = NULL
	},
	[DELETE_EMPLOYEE] = {
		.statement = NULL,
		.query = "call elimina_dipendente(?)",
		.required_params = 1,
		.params_type = (FIELD_TYPE[1]) { FIELD_TYPE_LONG },
		.result_columns = 0,
		.result_columns_type = NULL
	},
	[DELETE_SCREENING] = {
		.statement = NULL,
		.query = "call elimina_proiezione(?, ?, ?, ?)",
		.required_params = 4,
		.params_type = (FIELD_TYPE[4]) { FIELD_TYPE_LONG, FIELD_TYPE_LONG, FIELD_TYPE_DATE, FIELD_TYPE_TIME },
		.result_columns = 0,
		.result_columns_type = NULL
	},
	[DELETE_HALL] = {
		.statement = NULL,
		.query = "call elimina_sala(?, ?)",
		.required_params = 2,
		.params_type = (FIELD_TYPE[2]) { FIELD_TYPE_LONG, FIELD_TYPE_LONG },
		.result_columns = 0,
		.result_columns_type = NULL
	},
	[DELETE_SHIFT] = {
		.statement = NULL,
		.query = "call elimina_turno(?, ?, ?)",
		.required_params = 3,
		.params_type = (FIELD_TYPE[3]) { FIELD_TYPE_LONG, FIELD_TYPE_STRING, FIELD_TYPE_TIME },
		.result_columns = 0,
		.result_columns_type = NULL
	},
	[ADD_CINEMA] = {
		.statement = NULL,
		.query = "call inserisci_cinema(?, ?, ?)",
		.required_params = 3,
		.params_type = (FIELD_TYPE[3]) { FIELD_TYPE_STRING, FIELD_TYPE_TIME, FIELD_TYPE_TIME },
		.result_columns = 0,
		.result_columns_type = NULL
	},
	[ADD_EMPLOYEE] = {
		.statement = NULL,
		.query = "call inserisci_dipendente(?, ?, ?)",
		.required_params = 3,
		.params_type = (FIELD_TYPE[3]) { FIELD_TYPE_STRING, FIELD_TYPE_STRING, FIELD_TYPE_STRING },
		.result_columns = 0,
		.result_columns_type = NULL
	},
	[ADD_SCREENING] = {
		.statement = NULL,
		.query = "call inserisci_proiezione(?, ?, ?, ?, ?, ?)",
		.required_params = 6,
		.params_type = (FIELD_TYPE[6]) { FIELD_TYPE_LONG, FIELD_TYPE_LONG, FIELD_TYPE_DATE, FIELD_TYPE_TIME, FIELD_TYPE_DECIMAL, FIELD_TYPE_LONG },
		.result_columns = 0,
		.result_columns_type = NULL
	},
	[ADD_HALL] = {
		.statement = NULL,
		.query = "call inserisci_sala(?, ?, ?, ?)",
		.required_params = 4,
		.params_type = (FIELD_TYPE[4]) { FIELD_TYPE_LONG, FIELD_TYPE_LONG, FIELD_TYPE_LONG, FIELD_TYPE_LONG },
		.result_columns = 0,
		.result_columns_type = NULL
	},
	[ADD_SHIFT] = {
		.statement = NULL,
		.query = "call inserisci_turno(?, ?, ?, ?, ?)",
		.required_params = 5,
		.params_type = (FIELD_TYPE[5]) { FIELD_TYPE_LONG, FIELD_TYPE_STRING, FIELD_TYPE_TIME, FIELD_TYPE_TIME, FIELD_TYPE_LONG },
		.result_columns = 0,
		.result_columns_type = NULL
	},
	[GET_ALL_CINEMA] = {
		.statement = NULL,
		.query = "call mostra_cinema()",
		.required_params = 0,
		.params_type = NULL,
		.result_columns = 4,
		.result_columns_type = (FIELD_TYPE[4]) { FIELD_TYPE_LONG, FIELD_TYPE_STRING, FIELD_TYPE_TIME, FIELD_TYPE_TIME }
	},
	[REPORT_CINEMA_WITHOUT_USHERS] = {
		.statement = NULL,
		.query = "call mostra_cinema_senza_maschere()",
		.required_params = 0,
		.params_type = NULL,
		.result_columns = 5,
		.result_columns_type = (FIELD_TYPE[5]) { FIELD_TYPE_LONG, FIELD_TYPE_STRING, FIELD_TYPE_TIME, FIELD_TYPE_TIME, FIELD_TYPE_LONG }
	},
	[GET_EMPLOYEES] = {
		.statement = NULL,
		.query = "call mostra_dipendenti()",
		.required_params = 0,
		.params_type = NULL,
		.result_columns = 4,
		.result_columns_type = (FIELD_TYPE[4]) { FIELD_TYPE_LONG, FIELD_TYPE_STRING, FIELD_TYPE_STRING, FIELD_TYPE_STRING }
	},
	[GET_CINEMA_SCREENINGS] = {
		.statement = NULL,
		.query = "call mostra_palinsesto(?)",
		.required_params = 1,
		.params_type = (FIELD_TYPE[1]) { FIELD_TYPE_LONG },
		.result_columns = 10,
		.result_columns_type = (FIELD_TYPE[10]) { FIELD_TYPE_DATE, FIELD_TYPE_TIME, FIELD_TYPE_LONG, FIELD_TYPE_LONG, FIELD_TYPE_DECIMAL, FIELD_TYPE_LONG, FIELD_TYPE_STRING, FIELD_TYPE_TIME, FIELD_TYPE_STRING, FIELD_TYPE_STRING }
	},
	[GET_AVAILABLE_PROJECTIONISTS] = {
		.statement = NULL,
		.query = "call mostra_posti_disponibili(?, ?, ?, ?)",
		.required_params = 4,
		.params_type = (FIELD_TYPE[4]) { FIELD_TYPE_LONG, FIELD_TYPE_LONG, FIELD_TYPE_DATE, FIELD_TYPE_TIME },
		.result_columns = 2,
		.result_columns_type = (FIELD_TYPE[2]) { FIELD_TYPE_STRING, FIELD_TYPE_LONG }
	},
	[GET_SCREENINGS] = {
		.statement = NULL,
		.query = "call mostra_proiezioni()",
		.required_params = 0,
		.params_type = NULL,
		.result_columns = 9,
		.result_columns_type = (FIELD_TYPE[9]) { FIELD_TYPE_DATE, FIELD_TYPE_LONG, FIELD_TYPE_LONG, FIELD_TYPE_LONG, FIELD_TYPE_STRING, FIELD_TYPE_DECIMAL, FIELD_TYPE_TIME, FIELD_TYPE_TIME, FIELD_TYPE_LONG }
	},
	[REPORT_SCREENINGS_WITHOUT_PROJECTIONIST] = {
		.statement = NULL,
		.query = "call mostra_proiezioni_senza_proiezionista()",
		.required_params = 0,
		.params_type = NULL,
		.result_columns = 7,
		.result_columns_type = (FIELD_TYPE[7]) { FIELD_TYPE_LONG, FIELD_TYPE_LONG, FIELD_TYPE_DATE, FIELD_TYPE_TIME, FIELD_TYPE_DECIMAL, FIELD_TYPE_LONG, FIELD_TYPE_LONG }
	},
	[GET_AVAILABLE_PROJECTIONISTS] = {
		.statement = NULL,
		.query = "call mostra_proiezionisti_disponibili(?, ?, ?, ?)",
		.required_params = 4,
		.params_type = (FIELD_TYPE[4]) { FIELD_TYPE_LONG, FIELD_TYPE_LONG, FIELD_TYPE_DATE, FIELD_TYPE_TIME },
		.result_columns = 3,
		.result_columns_type = (FIELD_TYPE[3]) { FIELD_TYPE_LONG, FIELD_TYPE_STRING, FIELD_TYPE_STRING }
	},
	[GET_HALLS] = {
		.statement = NULL,
		.query = "call mostra_sale(?)",
		.required_params = 1,
		.params_type = (FIELD_TYPE[1]) { FIELD_TYPE_LONG },
		.result_columns = 4,
		.result_columns_type = (FIELD_TYPE[4]) { FIELD_TYPE_LONG, FIELD_TYPE_LONG, FIELD_TYPE_LONG, FIELD_TYPE_LONG }
	},
	[REPORT_RESERVATIONS_STATUS] = {
		.statement = NULL,
		.query = "call mostra_stato_prenotazioni()",
		.required_params = 0,
		.params_type = NULL,
		.result_columns = 5,
		.result_columns_type = (FIELD_TYPE[5]) { FIELD_TYPE_LONG, FIELD_TYPE_LONG, FIELD_TYPE_LONG, FIELD_TYPE_LONG, FIELD_TYPE_LONG }
	},
	[GET_SHIFTS] = {
		.statement = NULL,
		.query = "call mostra_turni()",
		.required_params = 0,
		.params_type = NULL,
		.result_columns = 8,
		.result_columns_type = (FIELD_TYPE[8]) { FIELD_TYPE_DATE, FIELD_TYPE_TIME, FIELD_TYPE_TIME, FIELD_TYPE_LONG, FIELD_TYPE_LONG, FIELD_TYPE_STRING, FIELD_TYPE_STRING, FIELD_TYPE_STRING }
	},
	[VALIDATE_BOOKING] = {
		.statement = NULL,
		.query = "call valida_prenotazione(?)",
		.required_params = 1,
		.params_type = (FIELD_TYPE[1]) { FIELD_TYPE_LONG },
		.result_columns = 0,
		.result_columns_type = NULL
	}
};
