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
			const int mask_size = 32;

			Bitmask mask(mask_size, false);
			vector<int> true_set;

			auto Check = [&]() {
				for (int i = 0; i < mask_size; ++i)
					if (find(true_set.begin(), true_set.end(), i) != true_set.end())
						Assert::IsTrue(mask.Check(i), L"Fail on IsTrue Set10");
					else
						Assert::IsFalse(mask.Check(i), L"Fail on IsFalse Set10");
			};

			true_set.push_back(10);
			mask.Set(10, true);
			Check();
			  
			true_set.push_back(16);
			mask.Set(16, true);
			Check();
			  
			true_set.erase(find(true_set.begin(), true_set.end(), 10));
			mask.Set(10, false);
			Check(); 
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
	
		TEST_METHOD(GetOffsetSmall) {
			const int mask_size = 32;

			Bitmask mask(mask_size, false);
			vector<size_t> true_set = { 2, 10, 16, 28, 30 };

			for (int i : true_set)
				mask.Set<true>(i);

			for (int i = 0; i < true_set.size(); ++i) 
				Assert::AreEqual(*(true_set.begin() + i), mask.GetOffset(i), L"", LINE_INFO());
			
		}

		TEST_METHOD(GetOffsetLarge) {
			const int mask_size = 1024;

			Bitmask mask(mask_size, false);  
			vector<unsigned> true_set = { 64, 105, 511, 512, 513, 542, 1023, };

			for (int i : true_set)
				mask.Set<true>(i);

			for (int i = 0; i < true_set.size(); ++i)
				Assert::AreEqual(*(true_set.begin() + i), mask.GetOffset(i), L"", LINE_INFO());
		} 
	};
}