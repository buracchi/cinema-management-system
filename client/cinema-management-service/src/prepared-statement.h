#pragma once

#include <stdint.h>
#include <mysql.h>

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
} statements_data[PREPARED_STATEMENT_NUMBER] = {
	[CANCEL_BOOKING] = { .statement = NULL, .query = "call annulla_prenotazione(?)" },
	[ASSIGN_PROJECTIONIST] = { .statement = NULL, .query = "call assegna_proiezionista(?, ?, ?, ?, ?)" },
	[BOOK_SEAT] = { .statement = NULL, .query = "call effettua_prenotazione(?, ?, ?, ?, ?, ?, ?, ?, ?, ?)" },
	[DELETE_CINEMA] = { .statement = NULL, .query = "call elimina_cinema(?)" },
	[DELETE_EMPLOYEE] = { .statement = NULL, .query = "call elimina_dipendente(?)" },
	[DELETE_SCREENING] = { .statement = NULL, .query = "call elimina_proiezione(?, ?, ?, ?)" },
	[DELETE_HALL] = { .statement = NULL, .query = "call elimina_sala(?, ?)" },
	[DELETE_SHIFT] = { .statement = NULL, .query = "call elimina_turno(?, ?, ?)" },
	[ADD_CINEMA] = { .statement = NULL, .query = "call inserisci_cinema(?, ?, ?)" },
	[ADD_EMPLOYEE] = { .statement = NULL, .query = "call inserisci_dipendente(?, ?, ?)" },
	[ADD_SCREENING] = { .statement = NULL, .query = "call inserisci_proiezione(?, ?, ?, ?, ?, ?)" },
	[ADD_HALL] = { .statement = NULL, .query = "call inserisci_sala(?, ?, ?, ?)" },
	[ADD_SHIFT] = { .statement = NULL, .query = "call inserisci_turno(?, ?, ?, ?, ?)" },
	[GET_ALL_CINEMA] = { .statement = NULL, .query = "call mostra_cinema()" },
	[REPORT_CINEMA_WITHOUT_USHERS] = { .statement = NULL, .query = "call mostra_cinema_senza_maschere()" },
	[GET_EMPLOYEES] = { .statement = NULL, .query = "call mostra_dipendenti()" },
	[GET_CINEMA_SCREENINGS] = { .statement = NULL, .query = "call mostra_palinsesto(?)" },
	[GET_AVAILABLE_PROJECTIONISTS] = { .statement = NULL, .query = "call mostra_posti_disponibili(?, ?, ?, ?)" },
	[GET_SCREENINGS] = { .statement = NULL, .query = "call mostra_proiezioni()" },
	[REPORT_SCREENINGS_WITHOUT_PROJECTIONIST] = { .statement = NULL, .query = "call mostra_proiezioni_senza_proiezionista()" },
	[GET_AVAILABLE_PROJECTIONISTS] = { .statement = NULL, .query = "call mostra_proiezionisti_disponibili(?, ?, ?, ?)" },
	[GET_HALLS] = { .statement = NULL, .query = "call mostra_sale(?)" },
	[REPORT_RESERVATIONS_STATUS] = { .statement = NULL, .query = "call mostra_stato_prenotazioni()" },
	[GET_SHIFTS] = { .statement = NULL, .query = "call mostra_turni()" },
	[VALIDATE_BOOKING] = { .statement = NULL, .query = "call valida_prenotazione(?)" }
};

extern MYSQL_STMT* get_prepared_stmt(cinema_management_service_t service, enum statement_operation operation);

extern const char* get_last_error(cinema_management_service_t service);
