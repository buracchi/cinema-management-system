#include <gtest/gtest.h>
#include <payment-service.h>

TEST(effettua_pagamento, return_buffer_pointer) {
	char buffer[256] = { };
	long long trash = 0;
	char garbage = 0;
	char* args[6] = { (char*) & trash, &garbage, &garbage, &garbage, &garbage, &garbage };
	unsigned long lengths[6] = { };
	UDF_INIT initid = { .ptr = buffer};
	UDF_ARGS udf_args = { .args = args, .lengths = lengths};
	unsigned long length;
	char is_null;
	char error;
	char* result = effettua_pagamento(&initid, &udf_args, nullptr, &length, &is_null, &error);
	ASSERT_EQ(result, buffer);
}

TEST(effettua_rimborso, return_zero) {
	int result = effettua_rimborso(nullptr, nullptr, nullptr, nullptr);
	ASSERT_EQ(result, 0);
}
