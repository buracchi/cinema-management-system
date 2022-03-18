#include "domain/hall.h"

typedef struct hall {
	uint8_t number;
	cinema_t cinema;
};

extern uint8_t get_number(hall_t hall) {
	return hall->number;
}

extern cinema_t get_cinema(hall_t hall) {
	return hall->cinema;
}
