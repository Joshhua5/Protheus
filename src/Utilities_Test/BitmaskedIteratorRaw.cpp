#pragma once

#include "stdafx.h"
#include "CppUnitTest.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;


namespace Utilities_Test
{
	using namespace Pro;
	using namespace Util;

	TEST_CLASS(BitmaskedIteratorRawTest)
	{
		template<typename T>
		struct array_deleter { void operator ()(T const * p) { delete[] p; } };

		static const int BUFFER_SIZE = 1024;  
		 
	public:

		TEST_METHOD(CreateIterator) {
			shared_ptr<int[]> data(new int[BUFFER_SIZE], array_deleter<int>());
			for (int i = 0; i < BUFFER_SIZE; ++i)
				data[i] = i;

			Bitmask mask(BUFFER_SIZE, true); 
			BitmaskedIteratorRaw iterator(&data[0], mask, BUFFER_SIZE, sizeof(int));

			Assert::AreEqual(&data[0], (int*)iterator.Read()); 
		}
		
		TEST_METHOD(ReadWithBitmask) {
			shared_ptr<int[]> data(new int[BUFFER_SIZE], array_deleter<int>());
			for (int i = 0; i < BUFFER_SIZE; ++i)
				data[i] = i;

			Bitmask mask(BUFFER_SIZE, true);
			BitmaskedIteratorRaw iterator(&data[0], mask, BUFFER_SIZE, sizeof(int));

			// Skip the first element
			mask.Set<false>(0);

			// The next element should be 1
			Assert::AreEqual(1, *static_cast<int*>(iterator.Read()));  
		} 

		TEST_METHOD(ReadLastWithBitmask) {
			shared_ptr<int[]> data(new int[BUFFER_SIZE], array_deleter<int>());
			for (int i = 0; i < BUFFER_SIZE; ++i)
				data[i] = i;

			Bitmask mask(BUFFER_SIZE, false);
			BitmaskedIteratorRaw iterator(&data[0], mask, BUFFER_SIZE, sizeof(int));

			// Skip the first element
			mask.Set<true>(BUFFER_SIZE - 1);

			// The next element should be 1
			Assert::AreEqual(BUFFER_SIZE - 1, *static_cast<int*>(iterator.Read()));
		}

		TEST_METHOD(ReadOverflowWithBitmask) {
			shared_ptr<int[]> data(new int[BUFFER_SIZE], array_deleter<int>());
			for (int i = 0; i < BUFFER_SIZE; ++i)
				data[i] = i;

			Bitmask mask(BUFFER_SIZE, false);
			BitmaskedIteratorRaw iterator(&data[0], mask, BUFFER_SIZE, sizeof(int));

			// Skip the first element
			mask.Set<true>(1);
			 
			// The next element should be 1
			Assert::AreEqual(1, *static_cast<int*>(iterator.Read()));

			// The rest of the bitmask is 0, so the next read should overflow and return nullptr
			Assert::AreEqual((void*)nullptr, iterator.Read());
		}
	};
}