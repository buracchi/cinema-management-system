#pragma once

#include <stdint.h>

typedef struct film* film_t;

extern int32_t film_get_id(film_t film);

extern char* film_get_name(film_t film);

extern char* film_get_running_time(film_t film);

extern char* film_get_film_studio(film_t film);

extern char* film_get_cast(film_t film);
