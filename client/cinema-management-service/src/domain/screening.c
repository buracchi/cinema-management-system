#include "domain/screening.h"

struct screening {
	hall_t hall;
	char date[11];	//'YYYY-MM-DD' format
	char time[9];	//'hh:mm:ss' format
	uint64_t price_decimal : 50;
	uint8_t price_cents;
	film_t film;
	employee_t projectionist;
};

extern hall_t screening_get_hall(screening_t screening) {
	return screening->hall;
}

extern char* screening_get_date(screening_t screening) {
	return screening->date;
}

extern char* screening_get_time(screening_t screening) {
	return screening->time;
}

extern uint64_t screening_get_price_decimal(screening_t screening) {
	return screening->price_decimal;
}

extern uint8_t screening_get_price_cents(screening_t screening) {
	return screening->price_cents;
}

extern film_t screening_get_film(screening_t screening) {
	return screening->film;
}

extern employee_t screening_get_projectionist(screening_t screening) {
	return screening->projectionist;
}
