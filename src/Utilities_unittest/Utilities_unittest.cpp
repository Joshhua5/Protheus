// Utilities_unittest.cpp : Defines the entry point for the console application.
//

#include "gtest/gtest.h"

#pragma comment(lib, "gtestd.lib")

int main(int argc, char* argv[])
{
	testing::InitGoogleTest(&argc, argv);
	int ret = RUN_ALL_TESTS();
	getchar();
	return 0;
}

