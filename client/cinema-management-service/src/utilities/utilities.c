#include "utilities.h"

#include <string.h>

int string_comparer(const char* str1, const char* str2, bool* result) {
	*result = !strcmp(str1, str2);
	return 0;
}
