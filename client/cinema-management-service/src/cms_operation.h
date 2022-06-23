#pragma once

#include <stddef.h>

#include "cms/cms.h"

// The current implementation limits to 16 the maximum number of fields returned from an operation

#define CMS_RESULT_BASE_BITMAP_INFO(s, m) (struct cms_result_bitmap) { ((size_t)&(((s*)0)->m)), (sizeof(((s*)0)->m)) }

#define CMS_RESULT_BITMAP_COUNT_VALS(N1, N2, N3, N4, N5, N6, N7, N8, N9, N10, N11, N12, N13, N14, N15, N16, N, ...) N
#define CMS_RESULT_BITMAP_COUNT_ARGS(...) CMS_RESULT_BITMAP_COUNT_VALS(__VA_ARGS__, 16, 15, 14, 13, 12, 11, 10, 09, 08, 07, 06, 05, 04, 03, 02, 01)

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
#define CMS_RESULT_BITMAP_CONCAT_MACRONAME(N) CMS_RESULT_BITMAP_INFO_ ## N
#define CMS_RESULT_BITMAP_MACRONAME(N) CMS_RESULT_BITMAP_CONCAT_MACRONAME(N)
#define CMS_RESULT_BITMAP_INFO(s, ...) CMS_RESULT_BITMAP_MACRONAME(CMS_RESULT_BITMAP_COUNT_ARGS(__VA_ARGS__))(s, __VA_ARGS__)

#define CMS_REQUEST_PARAM_INFO(ptr) (struct cms_request_param) { (void*)&(ptr), sizeof(ptr) }

struct cms_request_param {
	void* ptr;
	size_t size;
};

struct cms_result_bitmap {
	size_t offset;
	size_t size;
};

enum cms_operation {
	ABORT_BOOKING,
	ADD_CINEMA,
	ADD_EMPLOYEE,
	ADD_HALL,
	ADD_SCREENING,
	ADD_SHIFT,
	ASSIGN_PROJECTIONIST,
	CANCEL_BOOKING,
	COMMIT_BOOKING,
	CREATE_BOOKING,
	DELETE_CINEMA,
	DELETE_EMPLOYEE,
	DELETE_HALL,
	DELETE_SCREENING,
	DELETE_SHIFT,
	GET_ALL_CINEMA,
	GET_AVAILABLE_PROJECTIONISTS,
	GET_AVAILABLE_SEATS,
	GET_CINEMA_SCREENINGS,
	GET_EMPLOYEES,
	GET_HALLS,
	GET_MOVIES,
	GET_SCREENINGS,
	GET_SHIFTS,
	GET_CINEMA_WITHOUT_ENOUGH_USHERS,
	GET_MONTHLY_RESERVATIONS_STATUS,
	GET_SCREENINGS_WITHOUT_PROJECTIONIST,
	VALIDATE_BOOKING,
	OPERATIONS_NUMBER
};

extern int cms_operation_execute(cms_t, enum cms_operation, struct cms_request_param*, struct cms_response**, struct cms_result_bitmap*);
