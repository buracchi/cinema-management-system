#include "domain/hall.h"

struct hall {
	uint8_t number;
	cinema_t cinema;
};

extern uint8_t hall_get_number(hall_t hall) {
	return hall->number;
}

extern cinema_t hall_get_cinema(hall_t hall) {
	return hall->cinema;
}
