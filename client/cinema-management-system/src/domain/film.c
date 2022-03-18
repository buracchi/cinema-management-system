#include "domain/film.h"

struct film {
	uint32_t id;
	char* name;
	char running_time[9];	// 'hh:mm:ss' format
	char* film_studio;
	char* cast;
};

extern uint32_t get_id(film_t film) {
	return film->id;
}

extern char* get_name(film_t film) {
	return film->name;
}

extern char get_running_time(film_t film) {
	return film->running_time;
}

extern char* get_film_studio(film_t film) {
	return film->film_studio;
}

extern char* get_cast(film_t film) {
	return film->cast;
}
