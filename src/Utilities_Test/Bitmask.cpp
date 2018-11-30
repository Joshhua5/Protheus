#pragma once

#include "stdafx.h"
#include "CppUnitTest.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;


namespace Utilities_Test
{
	using namespace Pro;
	using namespace Util;
	using namespace std;

	TEST_CLASS(BitmaskTest)
	{
	public:

		TEST_METHOD(SetCheckSmallMask) {
			Bitmask mask(32, false);
			 
			mask.Set(10, true); 
			for (int i = 0; i < 32; ++i) {
				if (i == 10)
					Assert::IsTrue(mask.Check(i), L"Fail on IsTrue Set10");
				else
					Assert::IsFalse(mask.Check(i), L"Fail on IsFalse Set10");
			} 
			 

			mask.Set(16, true);
			for (int i = 0; i < 32; ++i) {
				if (i == 10 || i == 16)
					Assert::IsTrue(mask.Check(i), L"Fail on IsTrue Set16");
				else
					Assert::IsFalse(mask.Check(i), L"Fail on IsFalse Set16");
			}
			
			mask.Set(10, false);
			for (int i = 0; i < 32; ++i) {
				if (i == 16)
					Assert::IsTrue(mask.Check(i), L"Fail on IsTrue Unset10");
				else
					Assert::IsFalse(mask.Check(i), L"Fail on IsFalse Unset10");
			}
		} 

		TEST_METHOD(SetCheckLargeMask) {
			Bitmask mask(1024, false);
			 
			vector<unsigned> test_set = { 64, 105, 542, 1023, 512, 511, 513 };

			for (auto i : test_set) {
				mask.Set(i, true); 
			}

			// Check each bit
			for (int i = 0; i < 1024; ++i) {
				// Check if any of the test set ones are true
				bool skip = false;
				for (auto idx : test_set) {
					if (idx == i) {
						Assert::IsTrue(mask.Check(i)); 
						skip = true;
					}
				}

				if (skip)
					continue;

				// Otherwise they should be false
				Assert::IsFalse(mask.Check(i)); 
			} 
			
			// Set all even values to false
			for (auto i : test_set) {
				mask.Set(i, i % 2);
			}
			
			// Check each bit
			for (int i = 0; i < 1024; ++i) {
				// Check if any of the test set ones are true
				bool skip = false;
				for (auto idx : test_set) {
					if (idx == i && i % 2) {
						Assert::IsTrue(mask.Check(i));
						skip = true;
					}
				}

				if (skip)
					continue;

				// Otherwise they should be false
				Assert::IsFalse(mask.Check(i));
			}
		} 
	};
}