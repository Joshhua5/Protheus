#pragma once

#include "stdafx.h"
#include "CppUnitTest.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;


namespace Utilities_Test
{
	using namespace Pro;
	using namespace Util;

	TEST_CLASS(BitmaskedIteratorTest)
	{  
		static const int BUFFER_SIZE = 1024;  
		 
	public:

		TEST_METHOD(CreateIterator) {
			int* data = new int[BUFFER_SIZE];

			for (int i = 0; i < BUFFER_SIZE; ++i)
				data[i] = i;

			Bitmask mask(BUFFER_SIZE, true);
			BitmaskedIterator<int> iterator(data, mask, BUFFER_SIZE);

			Assert::AreEqual(&data[0], iterator.Read());
			delete[] data;
		}
		
		TEST_METHOD(ReadWithBitmask) {
			int* data = new int[BUFFER_SIZE];

			for (int i = 0; i < BUFFER_SIZE; ++i)
				data[i] = i;

			Bitmask mask(BUFFER_SIZE, true);
			BitmaskedIterator<int> iterator(data, mask, BUFFER_SIZE);

			// Skip the first element
			mask.Set<false>(0);

			// The next element should be 1
			Assert::AreEqual(1, *static_cast<int*>(iterator.Read()));
			delete[] data;
		} 

		TEST_METHOD(ReadLastWithBitmask) {
			int* data = new int[BUFFER_SIZE];

			for (int i = 0; i < BUFFER_SIZE; ++i)
				data[i] = i;

			Bitmask mask(BUFFER_SIZE, false);
			BitmaskedIterator<int> iterator(data, mask, BUFFER_SIZE);

			// Skip the first element
			mask.Set<true>(BUFFER_SIZE - 1);

			// The next element should be 1
			Assert::AreEqual(BUFFER_SIZE - 1, *iterator.Read());
			delete[] data;
		}
		
		TEST_METHOD(HasNext) {
			int* data = new int[BUFFER_SIZE];

			for (int i = 0; i < BUFFER_SIZE; ++i)
				data[i] = i;

			Bitmask mask(BUFFER_SIZE, false);
			BitmaskedIterator<int> iterator(data, mask, BUFFER_SIZE);

			// Skip the first element
			mask.Set<true>(BUFFER_SIZE - 1);

			// The next element should be 1
			iterator.Read();

			Assert::IsFalse(iterator.HasNext());

			delete[] data;
		}

		TEST_METHOD(ReadOverflowWithBitmask) { 
			int* data = new int[BUFFER_SIZE];

			for (int i = 0; i < BUFFER_SIZE; ++i)
				data[i] = i;

			Bitmask mask(BUFFER_SIZE, false);
			BitmaskedIterator<int> iterator(data, mask, BUFFER_SIZE);

			// Skip the first element
			mask.Set<true>(1);
			 
			// The next element should be 1
			Assert::AreEqual(1, *iterator.Read());

			// The rest of the bitmask is 0, so the next read should overflow and return nullptr
			Assert::AreEqual((int*)nullptr, iterator.Read());

			delete[] data;
		}
	};
}