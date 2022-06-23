#include <gtest/gtest.h>
#include <payment-service.h>

TEST(EFFETTUA_PAGAMENTO, return_buffer_pointer) {
	char buffer[256] = { };
	char* args[5] = { buffer, buffer, buffer, buffer, buffer };
	unsigned long lengths[5] = { };
	UDF_INIT initid = { .ptr = buffer};
	UDF_ARGS udf_args = { .args = args, .lengths = lengths};
	unsigned long length;
	char is_null;
	char error;
	char* result = EFFETTUA_PAGAMENTO(&initid, &udf_args, nullptr, &length, &is_null, &error);
	ASSERT_EQ(result, buffer);
}

TEST(EFFETTUA_RIMBORSO, return_zero) {
	long long result = EFFETTUA_RIMBORSO(nullptr, nullptr, nullptr, nullptr);
	ASSERT_EQ(result, 0);
}
