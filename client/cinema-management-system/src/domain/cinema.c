#include "domain/cinema.h"

struct cinema {
	int32_t id;
	char* address;
	char opening_time[9];	// "hh:mm:ss" format
	char closing_time[9];	// "hh:mm:ss" format
};

extern int32_t cinema_get_id(cinema_t cinema) {
	return cinema->id;
}

extern char* cinema_get_address(cinema_t cinema) {
	return cinema->address;
}

extern char* cinema_get_opening_time(cinema_t cinema) {
	return cinema->opening_time;
}

extern char* cinema_get_closing_time(cinema_t cinema) {
	return cinema->closing_time;
}
