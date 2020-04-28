// This sample shows how to write a simple unit test for a function,
// using Google C++ testing framework.
//
// Writing a unit test using Google C++ testing framework is easy as 1-2-3:


// Step 1. Include necessary header files such that the stuff your
// test logic needs is declared.
//
// Don't forget gtest.h, which declares the testing framework.

// Step 2. Use the TEST macro to define your tests.
//
// TEST has two parameters: the test case name and the test name.
// After using the macro, you should define your test logic between a
// pair of braces.  You can use a bunch of macros to indicate the
// success or failure of a test.  EXPECT_TRUE and EXPECT_EQ are
// examples of such macros.  For a complete list, see gtest.h

#include "gtest/gtest.h"

#include <string>

#include "inc/atm.h"
/****************************************************/
/*                    DUMMY TEST                    */
/****************************************************/
TEST( AtmGroupDummy, TestDummy )
{
	int expected = 1;
	int actual = 1;

	EXPECT_EQ( expected, actual );

	GTEST_SUCCEED();
}
/*****************************************************/
/*               GROUP TEST ACCCOUNT                 */
/*****************************************************/
TEST( AtmGroupAccount, NewAccount )
{
	std::string dummy = "TestFile";
	
	EXPECT_TRUE( is_new_account( dummy.c_str() ));

	GTEST_SUCCEED();
}
/*****************************************************/
/*                 GROUP TEST PIN                    */
/*****************************************************/
TEST( AtmGroupPin, PinValid )
{
	std::string pin = "0123";
	EXPECT_TRUE( is_pin_correct( pin.c_str() ));

	GTEST_SUCCEED();
}

TEST( AtmGroupPin, PinInvalid )
{
	std::string pin = "012";
	EXPECT_FALSE( is_pin_correct( pin.c_str() ));

	GTEST_SUCCEED();
}
/*****************************************************/
/*                 MAIN FUCNTION                     */
/*****************************************************/
int main( int argc, char **argv )
{
	::testing::InitGoogleTest( &argc, argv );
	return RUN_ALL_TESTS();
}