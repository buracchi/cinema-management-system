#include "buracchi/libcommon/utilities/try.h"

int main(int argc, const char** argv) {
	try(1, 0, fail);
	return 0;
fail:
	return 1;
}
