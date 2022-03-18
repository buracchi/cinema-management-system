#include "domain/shift.h"

struct shift {
	employee_t employee;
	enum weekday weekday;
	char opening_time[9];	// "hh:mm:ss" format
	char closing_time[9];	// "hh:mm:ss" format
	cinema_t cinema;
};

extern employee_t shift_get_employee(shift_t shift) {
	return shift->employee;
}

extern enum weekday shift_get_weekday(shift_t shift) {
	return shift->weekday;
}

extern char* shift_get_opening_time(shift_t shift) {
	return shift->opening_time;
}

extern char* shift_get_closing_time(shift_t shift) {
	return shift->closing_time;
}

extern cinema_t shift_get_cinema(shift_t shift) {
	return shift->cinema;
}
