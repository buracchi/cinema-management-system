#include "domain/hall.h"

typedef struct screening {
	hall_t hall;
	char date[11];	//'YYYY-MM-DD' format
	char time[9];	//'hh:mm:ss' format
	uint64_t price_decimal;
	uint8_t price_cents;
	film_t film;
	employee_t projectionist;
};

extern hall_t get_hall(screening_t screening) {
	return screening->hall;
}

extern char get_date(screening_t screening) {
	return screening->date;
}

extern char get_time(screening_t screening) {
	return screening->time;
}

extern uint64_t get_price_decimal(screening_t screening) {
	return screening->price_decimal;
}

extern uint8_t get_price_cents(screening_t screening) {
	return screening->price_cents;
}

extern film_t get_film(screening_t screening) {
	return screening->film;
}

extern employee_t get_projectionist(screening_t screening) {
	return screening->projectionist;
}
