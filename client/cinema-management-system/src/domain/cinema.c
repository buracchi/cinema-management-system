#include "domain/cinema.h"

struct cinema {
	int32_t id;
	char* address;
	char opening_time[9];	// "hh:mm:ss" format
	char closing_time[9];	// "hh:mm:ss" format
};

extern int32_t get_id(cinema_t cinema) {
	return cinema->id;
}

extern char* get_address(cinema_t cinema) {
	return cinema->address;
}

extern char* get_opening_time(cinema_t cinema) {
	return cinema->opening_time;
}

extern char* get_closing_time(cinema_t cinema) {
	return cinema->closing_time;
}
