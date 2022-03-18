#pragma once

#include <errno.h>

#include <buracchi/libcommon/utilities/utilities.h>

#define _api(name, namespace) extern errno_t namespace##_##name( \
									cinema_management_system_t system, \
									const struct namespace##_##name##_request* request, \
									struct namespace##_##name##_response response)
#define API(name) _api(name, cinema_management_system)

typedef struct cinema_management_system* cinema_management_system_t;

extern cinema_management_system_t cinema_management_system_init();

struct login_request { void* credentials; };
struct login_response { void* param; };
extern errno_t login(cinema_management_system_t system, struct login_request request, struct login_response* response);

struct logout_request { void* param; };
extern errno_t logout(cinema_management_system_t system, struct logout_request request);

extern errno_t cinema_management_system_destroy(cinema_management_system_t argparser);
