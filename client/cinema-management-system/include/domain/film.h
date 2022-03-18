#pragma once

#include <stdint.h>

typedef struct film* film_t;

extern uint32_t get_id(film_t film);

extern char* get_name(film_t film);

extern char get_running_time(film_t film);

extern char* get_film_studio(film_t film);

extern char* get_cast(film_t film);
