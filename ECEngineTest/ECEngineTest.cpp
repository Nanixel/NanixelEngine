// ECEngineTest.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "gtest/gtest.h"

int AddTwoNumber(int firstNumber, int secondNumber) {
	return firstNumber + secondNumber;
}


TEST(NumbersTest, AddTwoNumbers) {

	EXPECT_EQ(4, AddTwoNumber(2, 2));

}

