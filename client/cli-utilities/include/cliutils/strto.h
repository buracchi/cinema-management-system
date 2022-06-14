#pragma once

#include <stdint.h>

enum strto_errno {
    STRTO_SUCCESS,
    STRTO_OVERFLOW,
    STRTO_UNDERFLOW,
    STRTO_INCONVERTIBLE
};

/* Convert string str to int32_t out.
 *
 * @param[out] out The converted int32_t. Cannot be NULL.
 *
 * @param[in] str Input string to be converted.
 *
 *     The format is the same as strtol,
 *     except that the following are inconvertible:
 *
 *     - empty string
 *     - leading whitespace
 *     - any trailing characters that are not part of the number
 *
 *     Cannot be NULL.
 *
 * @param[in] base Base to interpret string in. Same range as strtol (2 to 36).
 *
 * @return Indicates if the operation succeeded, or why it failed.
 */
enum strto_errno strtoint32(int32_t* out, char* str, int base);

enum strto_errno strtouint16(uint16_t* out, char* str, int base);
