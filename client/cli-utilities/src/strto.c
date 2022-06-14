#include "cliutils/strto.h"

#include <ctype.h>
#include <errno.h>
#include <stdlib.h>

enum strto_errno strtoint32(int32_t* out, const char* str, int base) {
    char* end;
    if (str[0] == '\0' || isspace(str[0])) {
        return STRTO_INCONVERTIBLE;
    }
    errno = 0;
    long long result = strtoll(str, &end, base);
    if (result > INT32_MAX) {
        return STRTO_OVERFLOW;
    }
    if (result < INT32_MIN) {
        return STRTO_UNDERFLOW;
    }
    if (*end != '\0') {
        return STRTO_INCONVERTIBLE;
    }
    *out = (int32_t)result;
    return STRTO_SUCCESS;
}

enum strto_errno strtouint16(uint16_t* out, const char* str, int base) {
    char* end;
    if (str[0] == '\0' || isspace(str[0])) {
        return STRTO_INCONVERTIBLE;
    }
    errno = 0;
    long long result = strtoll(str, &end, base);
    if (result > UINT16_MAX)
        return STRTO_OVERFLOW;
    if (result < 0)
        return STRTO_UNDERFLOW;
    if (*end != '\0')
        return STRTO_INCONVERTIBLE;
    *out = (uint16_t)result;
    return STRTO_SUCCESS;
}
