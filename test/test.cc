#include "gtest/gtest.h"


TEST(Tests1, SampleTest)
{
	int expected = 10;
	int actual = 10;
	EXPECT_EQ( expected, actual );
}


int main(int argc, char **argv)
{
	::testing::InitGoogleTest( &argc, argv );
	return RUN_ALL_TESTS();
}