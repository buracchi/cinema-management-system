#pragma once

#include <stdint.h>
#include <mysql.h>

#include "cms.h"

#define FIELD_TYPE enum enum_field_types

// The current implementation limits to 32 the maximum number of colums returned in a result set from a prepared statement

#define CMS_RESULT_BASE_BITMAP_INFO(s, m) (struct cms_result_bitmap) { ((size_t)&(((s*)0)->m)), (sizeof(((s*)0)->m)) }

#define CMS_RESULT_BITMAP_COUNT_VALS(N1, N2, N3, N4, N5, N6, N7, N8, N9, N10, N11, N12, N13, N14, N15, N16, N17, N18, N19, N20, N21, N22, N23, N24, N25, N26, N27, N28, N29, N30, N31, N32, N, ...) N
#define CMS_RESULT_BITMAP_COUNT_ARGS(...) CMS_RESULT_BITMAP_COUNT_VALS(__VA_ARGS__, 32, 31, 30, 29, 28, 27, 26, 25, 24, 23, 22, 21, 20, 19, 18, 17, 16, 15, 14, 13, 12, 11, 10, 09, 08, 07, 06, 05, 04, 03, 02, 01)

#define CMS_RESULT_BITMAP_INFO_01(s, m) CMS_RESULT_BASE_BITMAP_INFO(s, m)
#define CMS_RESULT_BITMAP_INFO_02(s, m, ...) CMS_RESULT_BASE_BITMAP_INFO(s, m), CMS_RESULT_BITMAP_INFO_01(s, __VA_ARGS__)
#define CMS_RESULT_BITMAP_INFO_03(s, m, ...) CMS_RESULT_BASE_BITMAP_INFO(s, m), CMS_RESULT_BITMAP_INFO_02(s, __VA_ARGS__)
#define CMS_RESULT_BITMAP_INFO_04(s, m, ...) CMS_RESULT_BASE_BITMAP_INFO(s, m), CMS_RESULT_BITMAP_INFO_03(s, __VA_ARGS__)
#define CMS_RESULT_BITMAP_INFO_05(s, m, ...) CMS_RESULT_BASE_BITMAP_INFO(s, m), CMS_RESULT_BITMAP_INFO_04(s, __VA_ARGS__)
#define CMS_RESULT_BITMAP_INFO_06(s, m, ...) CMS_RESULT_BASE_BITMAP_INFO(s, m), CMS_RESULT_BITMAP_INFO_05(s, __VA_ARGS__)
#define CMS_RESULT_BITMAP_INFO_07(s, m, ...) CMS_RESULT_BASE_BITMAP_INFO(s, m), CMS_RESULT_BITMAP_INFO_06(s, __VA_ARGS__)
#define CMS_RESULT_BITMAP_INFO_08(s, m, ...) CMS_RESULT_BASE_BITMAP_INFO(s, m), CMS_RESULT_BITMAP_INFO_07(s, __VA_ARGS__)
#define CMS_RESULT_BITMAP_INFO_09(s, m, ...) CMS_RESULT_BASE_BITMAP_INFO(s, m), CMS_RESULT_BITMAP_INFO_08(s, __VA_ARGS__)
#define CMS_RESULT_BITMAP_INFO_10(s, m, ...) CMS_RESULT_BASE_BITMAP_INFO(s, m), CMS_RESULT_BITMAP_INFO_09(s, __VA_ARGS__)
#define CMS_RESULT_BITMAP_INFO_11(s, m, ...) CMS_RESULT_BASE_BITMAP_INFO(s, m), CMS_RESULT_BITMAP_INFO_10(s, __VA_ARGS__)
#define CMS_RESULT_BITMAP_INFO_12(s, m, ...) CMS_RESULT_BASE_BITMAP_INFO(s, m), CMS_RESULT_BITMAP_INFO_11(s, __VA_ARGS__)
#define CMS_RESULT_BITMAP_INFO_13(s, m, ...) CMS_RESULT_BASE_BITMAP_INFO(s, m), CMS_RESULT_BITMAP_INFO_12(s, __VA_ARGS__)
#define CMS_RESULT_BITMAP_INFO_14(s, m, ...) CMS_RESULT_BASE_BITMAP_INFO(s, m), CMS_RESULT_BITMAP_INFO_13(s, __VA_ARGS__)
#define CMS_RESULT_BITMAP_INFO_15(s, m, ...) CMS_RESULT_BASE_BITMAP_INFO(s, m), CMS_RESULT_BITMAP_INFO_14(s, __VA_ARGS__)
#define CMS_RESULT_BITMAP_INFO_16(s, m, ...) CMS_RESULT_BASE_BITMAP_INFO(s, m), CMS_RESULT_BITMAP_INFO_15(s, __VA_ARGS__)
#define CMS_RESULT_BITMAP_INFO_17(s, m, ...) CMS_RESULT_BASE_BITMAP_INFO(s, m), CMS_RESULT_BITMAP_INFO_16(s, __VA_ARGS__)
#define CMS_RESULT_BITMAP_INFO_18(s, m, ...) CMS_RESULT_BASE_BITMAP_INFO(s, m), CMS_RESULT_BITMAP_INFO_17(s, __VA_ARGS__)
#define CMS_RESULT_BITMAP_INFO_19(s, m, ...) CMS_RESULT_BASE_BITMAP_INFO(s, m), CMS_RESULT_BITMAP_INFO_18(s, __VA_ARGS__)
#define CMS_RESULT_BITMAP_INFO_20(s, m, ...) CMS_RESULT_BASE_BITMAP_INFO(s, m), CMS_RESULT_BITMAP_INFO_19(s, __VA_ARGS__)
#define CMS_RESULT_BITMAP_INFO_21(s, m, ...) CMS_RESULT_BASE_BITMAP_INFO(s, m), CMS_RESULT_BITMAP_INFO_20(s, __VA_ARGS__)
#define CMS_RESULT_BITMAP_INFO_22(s, m, ...) CMS_RESULT_BASE_BITMAP_INFO(s, m), CMS_RESULT_BITMAP_INFO_21(s, __VA_ARGS__)
#define CMS_RESULT_BITMAP_INFO_23(s, m, ...) CMS_RESULT_BASE_BITMAP_INFO(s, m), CMS_RESULT_BITMAP_INFO_22(s, __VA_ARGS__)
#define CMS_RESULT_BITMAP_INFO_24(s, m, ...) CMS_RESULT_BASE_BITMAP_INFO(s, m), CMS_RESULT_BITMAP_INFO_23(s, __VA_ARGS__)
#define CMS_RESULT_BITMAP_INFO_25(s, m, ...) CMS_RESULT_BASE_BITMAP_INFO(s, m), CMS_RESULT_BITMAP_INFO_24(s, __VA_ARGS__)
#define CMS_RESULT_BITMAP_INFO_26(s, m, ...) CMS_RESULT_BASE_BITMAP_INFO(s, m), CMS_RESULT_BITMAP_INFO_25(s, __VA_ARGS__)
#define CMS_RESULT_BITMAP_INFO_27(s, m, ...) CMS_RESULT_BASE_BITMAP_INFO(s, m), CMS_RESULT_BITMAP_INFO_26(s, __VA_ARGS__)
#define CMS_RESULT_BITMAP_INFO_28(s, m, ...) CMS_RESULT_BASE_BITMAP_INFO(s, m), CMS_RESULT_BITMAP_INFO_27(s, __VA_ARGS__)
#define CMS_RESULT_BITMAP_INFO_29(s, m, ...) CMS_RESULT_BASE_BITMAP_INFO(s, m), CMS_RESULT_BITMAP_INFO_28(s, __VA_ARGS__)
#define CMS_RESULT_BITMAP_INFO_30(s, m, ...) CMS_RESULT_BASE_BITMAP_INFO(s, m), CMS_RESULT_BITMAP_INFO_29(s, __VA_ARGS__)
#define CMS_RESULT_BITMAP_INFO_31(s, m, ...) CMS_RESULT_BASE_BITMAP_INFO(s, m), CMS_RESULT_BITMAP_INFO_30(s, __VA_ARGS__)
#define CMS_RESULT_BITMAP_INFO_32(s, m, ...) CMS_RESULT_BASE_BITMAP_INFO(s, m), CMS_RESULT_BITMAP_INFO_31(s, __VA_ARGS__)

#define CMS_RESULT_BITMAP_CONCAT_MACRONAME(N) CMS_RESULT_BITMAP_INFO_ ## N
#define CMS_RESULT_BITMAP_MACRONAME(N) CMS_RESULT_BITMAP_CONCAT_MACRONAME(N)
#define CMS_RESULT_BITMAP_INFO(s, ...) CMS_RESULT_BITMAP_MACRONAME(CMS_RESULT_BITMAP_COUNT_ARGS(__VA_ARGS__))(s, __VA_ARGS__)

struct cms_request_param {
	void* ptr;
	size_t size;
};

struct cms_result_bitmap {
	size_t offset;
	size_t size;
};

enum cms_operation {
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

extern int cms_operation_execute(cms_t, enum cms_operation, struct cms_request_param*, struct cms_result_response**, struct cms_result_bitmap*);
