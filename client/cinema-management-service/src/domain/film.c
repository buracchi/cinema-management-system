#include "domain/film.h"

struct film {
	int32_t id;
	char* name;
	char running_time[9];	// 'hh:mm:ss' format
	char* film_studio;
	char* cast;
};

extern int32_t film_get_id(film_t film) {
	return film->id;
}

extern char* film_get_name(film_t film) {
	return film->name;
}

extern char* film_get_running_time(film_t film) {
	return film->running_time;
}

extern char* film_get_film_studio(film_t film) {
	return film->film_studio;
}

extern char* film_get_cast(film_t film) {
	return film->cast;
}
