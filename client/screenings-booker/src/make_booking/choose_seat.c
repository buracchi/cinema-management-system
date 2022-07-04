#include "make_booking.h"

#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>
#include <inttypes.h>

#include <buracchi/common/utilities/strto.h>
#include <buracchi/common/utilities/try.h>
#include <buracchi/common/utilities/utilities.h>
#include <cms/booking.h>
#include <cliutils/io.h>

#include "../core.h"

#ifdef _MSC_VER 
#define strncasecmp _strnicmp
#endif

static int get_hall_info(cms_t cms, struct booking_data* booking_data, uint64_t* num_rows, uint64_t* num_cols);
static inline int print_cinema_map(struct cms_seat* available_seats, uint64_t num_available_seats, uint64_t num_rows, uint64_t num_cols);

extern int choose_seat(cms_t cms, struct booking_data* booking_data) {
	struct cms_response response;
	struct cms_seat* available_seats;
	struct cms_screening screening = {
		.cinema_id = booking_data->cinema_id,
		.hall_number = booking_data->hall,
	};
	strcpy((char*)screening.date, booking_data->date);
	strcpy((char*)screening.start_time, booking_data->time);
	char input[INT32DSTR_LEN + 1];
	int32_t selected_number;
	bool selected_seat_available = false;
	bool back = false;
	while (true) {
		uint64_t num_rows = 0;
		uint64_t num_cols = 0;
		io_clear_screen();
		puts(title);
		switch (get_hall_info(cms, booking_data, &num_rows, &num_cols)) {
		case 1:
			return 1;
		case 2:
			return 0;
		}
		response = cms_get_available_seats(cms, &screening, &available_seats);
		if (response.fatal_error) {
			fprintf(stderr, "%s\n", response.error_message ? response.error_message : cms_get_error_message(cms));
			cms_destroy_response(&response);
			return 1;
		}
		if (response.error_message) {
			printf("%s\n", response.error_message);
			cms_destroy_response(&response);
			press_anykey();
			return 0;
		}
		printf("Data: %s\n", booking_data->date);
		printf("Orario: %s\n", booking_data->time);
		printf("Cinema: %s\n", booking_data->cinema_address);
		printf("Sala: %d\n", booking_data->hall);
		printf("Film: %s\n", booking_data->film_name);
		printf("Prezzo: %s\n", booking_data->price);
		print_cinema_map(available_seats, response.num_elements, num_rows, num_cols);
		get_input("Inserire lettera e numero (i.e. A1) per scegliere un posto o Q per tornare indietro: ", input, false);
		if (input[0] == 'Q' && input[1] == '\0') {
			back = true;
			break;
		}
		else if (isalpha(input[0])
			&& cmn_strto_int32(&selected_number, input + 1, 10) == 0
			&& selected_number > 0
			&& (uint64_t)selected_number <= num_cols) {
			char selected_row = input[0];
			for (uint64_t i = 0; i < response.num_elements; i++) {
				if (strncasecmp(&(available_seats[i].row[0]), &selected_row, 1) == 0
					&& available_seats[i].number == (uint32_t)selected_number) {
					selected_seat_available = true;
					booking_data->seat_row[0] = available_seats[i].row[0];
					booking_data->seat_number = available_seats[i].number;
					break;
				}
			}
			if (selected_seat_available) {
				break;
			}
		}
		cms_destroy_response(&response);
	}
	cms_destroy_response(&response);
	return back ? choose_screening(cms, booking_data) : make_payment(cms, booking_data);
}

static int get_hall_info(cms_t cms, struct booking_data* booking_data, uint64_t* num_rows, uint64_t* num_cols) {
	struct cms_response response;
	struct cms_hall* halls;
	response = cms_get_cinema_halls(cms, booking_data->cinema_id, &halls);
	if (response.fatal_error) {
		goto fail;
	}
	if (response.error_message) {
		printf("%s\n", response.error_message);
		cms_destroy_response(&response);
		press_anykey();
		return 2;
	}
	for (uint64_t i = 0; i < response.num_elements; i++) {
		if (halls[i].id == booking_data->hall) {
			*num_rows = halls[i].num_rows;
			*num_cols = halls[i].num_cols;
			break;
		}
	}
	cms_destroy_response(&response);
	return 0;
fail:
	fprintf(stderr, "%s\n", response.error_message ? response.error_message : cms_get_error_message(cms));
	cms_destroy_response(&response);
	return 1;
}

static inline int print_cinema_map(struct cms_seat* available_seats, uint64_t num_available_seats, uint64_t num_rows, uint64_t num_cols) {
	int ret;
	char* map;
	size_t map_rows = 12 + num_rows;
	size_t map_cols = 1 + max(21, (5 + 3 * num_cols));
	size_t map_size = map_rows * map_cols;
	try(map = malloc(map_size + 1), NULL, fail);
	memset(map, ' ', map_size);
	map[map_size] = 0;
	for (size_t i = 0; i < map_rows; i++) {
		map[i * map_cols + map_cols - 1] = '\n';
	}
	memcpy(&(map[2 * map_cols]), "Legenda:", sizeof("Legenda:") - 1);
	memcpy(&(map[3 * map_cols]), "o - posto disponibile", sizeof("o - posto disponibile") - 1);
	memcpy(&(map[4 * map_cols]), "x - posto riservato", sizeof("x - posto riservato") - 1);
	memcpy(&(map[7 * map_cols]), "  | ", 4);
	for (uint64_t i = 0; i < num_cols; i++) {
		char tmp[4];
		sprintf(tmp, "%s%" PRIu64 " ", i < 9 ? " " : "", i + 1);
		memcpy(&(map[7 * map_cols + 4 + 3 * i]), tmp, 3);
	}
	map[7 * map_cols + map_cols - 2] = '|';
	memcpy(&(map[8 * map_cols]), "--+-", 4);
	for (uint64_t i = 0; i < num_cols; i++) {
		memcpy(&(map[8 * map_cols + 4 + 3 * i]), "---", 3);
	}
	map[8 * map_cols + map_cols - 2] = '+';

	for (uint64_t i = 0; i < num_rows; i++) {
		char tmp[5];
		sprintf(tmp, " %c| ", (int)('A' + i));
		memcpy(&(map[(9 + i) * map_cols]), tmp, 4);
		for (uint64_t j = 0; j < num_cols; j++) {
			bool available = false;
			for (uint64_t k = 0; k < num_available_seats; k++) {
				if (available_seats[k].row[0] == (char)('A' + i)
					&& available_seats[k].number == (j + 1)) {
					available = true;
					break;
				}
			}
			map[(9 + i) * map_cols + 5 + (j * 3)] = available ? 'o' : 'x';
		}
		map[(9 + i) * map_cols + map_cols - 2] = '|';
	}
	memcpy(&(map[(9 + num_rows) * map_cols]), "--+", 3);
	for (size_t i = 3; i < map_cols - 2; i++) {
		map[(9 + num_rows) * map_cols + i] = '#';
	}
	map[(9 + num_rows) * map_cols + map_cols - 2] = '|';
	memcpy(&(map[(10 + num_rows) * map_cols]), "  |#", 4);
	memcpy(&(map[(10 + num_rows) * map_cols + map_cols / 2 - 3]), "SCHERMO", sizeof("SCHERMO") - 1);
	map[(10 + num_rows) * map_cols + map_cols - 3] = '#';
	map[(10 + num_rows) * map_cols + map_cols - 2] = '|';
	map[(11 + num_rows) * map_cols + 2] = '|';
	for (size_t i = 3; i < map_cols - 2; i++) {
		map[(11 + num_rows) * map_cols + i] = '#';
	}
	map[(11 + num_rows) * map_cols + map_cols - 2] = '|';
	ret = puts(map);
	free(map);
	return ret;
fail:
	return -1;
}
