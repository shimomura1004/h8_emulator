#include <gtest/gtest.h>
#include <gmock/gmock.h>

TEST(sumA1, normal)
{
	ASSERT_EQ(3, 1 + 2);
}

int main(int argc, char **argv)
{
	::testing::InitGoogleMock(&argc, argv);
	return RUN_ALL_TESTS();
}
