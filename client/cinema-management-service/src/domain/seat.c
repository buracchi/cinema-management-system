#include "domain/seat.h"

struct seat {
	char row;
	uint8_t number;
	hall_t hall;
};

extern char seat_get_row(seat_t seat) {
	return seat->row;
}

extern uint8_t seat_get_number(seat_t seat) {
	return seat->number;
}

extern hall_t seat_get_hall(seat_t seat) {
	return seat->hall;
}
