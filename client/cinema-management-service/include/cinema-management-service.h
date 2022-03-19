#pragma once

#include <errno.h>

#define _api(name, namespace) extern errno_t namespace##_##name( \
									cinema_management_service_t service, \
									const struct namespace##_##name##_request* request, \
									struct namespace##_##name##_response* response)
#define API(name) _api(name, cinema_management_service)

typedef struct cinema_management_service* cinema_management_service_t;

extern cinema_management_service_t cinema_management_service_init(const char* username, const char* password);

extern errno_t cinema_management_service_destroy(cinema_management_service_t service);
