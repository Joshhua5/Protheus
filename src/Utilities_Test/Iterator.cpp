#pragma once

#include "stdafx.h"
#include "CppUnitTest.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;


namespace Utilities_Test
{
	using namespace Pro;
	using namespace Util;

	TEST_CLASS(IteratorTest)
	{
		const size_t BUFFER_SIZE = 1024;
	public:

		TEST_METHOD(CreateIterator) {
			Buffer buffer(BUFFER_SIZE); 
			Iterator<char> iterator1(buffer.data<char>(), BUFFER_SIZE); 

			Assert::AreEqual(buffer.data<char>(), iterator1.Read());

		} 

		TEST_METHOD(HasNext) {
			Buffer buffer(BUFFER_SIZE); 
			Iterator<char> iterator(buffer.data<char>(), BUFFER_SIZE);

			unsigned readCount = 0;
			while (iterator.HasNext()) {
				iterator.Read();
				readCount++;
			}

			Assert::AreEqual(BUFFER_SIZE, readCount); 
		} 
	};
}