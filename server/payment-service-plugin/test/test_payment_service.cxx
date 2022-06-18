#include <gtest/gtest.h>
#include <payment-service.h>

TEST(effettua_rimborso, return_zero) {
	ASSERT_EQ(effettua_rimborso(nullptr, nullptr, nullptr, nullptr), 0);
}
