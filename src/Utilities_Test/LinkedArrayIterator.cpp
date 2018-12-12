#pragma once

#include "stdafx.h"
#include "CppUnitTest.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;


namespace Utilities_Test
{
	using namespace Pro;
	using namespace Util;

	TEST_CLASS(LinkedArrayIteratorTest)
	{  
		struct Complex {
			int x_, y_, z_, w_;
			
			Complex(){}

			Complex(int x, int y, int z, int w) {
				x_ = x;
				y_ = y;
				z_ = z;
				w_ = w;
			}
		};

	public: 
		TEST_METHOD(SimpleIteration) {
			// Test data
			LinkedArray<Complex> linkedArray(64);
			vector<int> test_set = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 }; 

			// Setup
			for (auto value : test_set)
				linkedArray.Emplace(value, value, value, value);
			 
			// Validate
			LinkedArrayIterator<Complex> it(linkedArray);
			for (auto value : test_set)
				Assert::AreEqual(value, it.Read()->x_); 
		}
		
		TEST_METHOD(MultipleIteration) {
			// Test data
			LinkedArray<Complex> linkedArray(64);

			// Setup
			for(int value = 0; value < 256; value++)
				linkedArray.Emplace(value, value, value, value);
			  
			// Validate
			LinkedArrayIterator<Complex> it(linkedArray);
			for (int value = 0; value < 256; value++)
				Assert::AreEqual(value, it.Read()->x_);
		}
		
		TEST_METHOD(HasNext) {
			// Test data
			LinkedArray<Complex> linkedArray(64);
			vector<int> test_set = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };

			// Setup
			for (auto value : test_set)
				linkedArray.Emplace(value, value, value, value);

			LinkedArrayIterator<Complex> it(linkedArray);
			for (auto value : test_set)
				it.Read();

			// Validate
			Assert::IsFalse(it.HasNext());
		}

		TEST_METHOD(FragmentedIteration) {
			// Test data
			LinkedArray<Complex> linkedArray(64);


			// Setup
			for (int i = 0; i < 64; ++i)
				linkedArray.Emplace(i, i, i, i);

			for (int i = 0; i < 32; i++)
				linkedArray.DeleteAt(i);

			// Validat
			LinkedArrayIterator<Complex> it(linkedArray);
			for (int i = 1; i < 64; i += 2) { 
				Assert::AreEqual(i, it.Read()->x_);
			}

			Assert::AreEqual(false, it.HasNext());
		}
	};
}