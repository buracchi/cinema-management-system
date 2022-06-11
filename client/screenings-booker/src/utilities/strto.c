#include "strto.h"

#include <ctype.h>
#include <errno.h>
#include <limits.h>
#include <stdlib.h>

enum strto_errno strtoint32(int32_t* out, char* str, int base) {
    char* end;
    if (str[0] == '\0' || isspace(str[0]))
        return STRTO_INCONVERTIBLE;
    errno = 0;
    long long result = strtoll(str, &end, base);
    /* Both checks are needed because INT_MAX == LONG_MAX is possible. */
    if (result > INT32_MAX || (errno == ERANGE && result == INT32_MAX))
        return STRTO_OVERFLOW;
    if (result < INT32_MIN || (errno == ERANGE && result == INT32_MIN))
        return STRTO_UNDERFLOW;
    if (*end != '\0')
        return STRTO_INCONVERTIBLE;
    *out = result;
    return STRTO_SUCCESS;
}
