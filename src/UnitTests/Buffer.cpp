#include "stdafx.h"
#include "CppUnitTest.h"
 


using namespace Pro;
using namespace ECS;
using namespace Microsoft::VisualStudio::CppUnitTestFramework;

#define BUFFER_SIZE (size_t)1024

TEST_CLASS(BUFFER_TEST) {
	 
	TEST_METHOD(CreateBuffer) {
		Buffer buffer(BUFFER_SIZE);

		Assert::AreNotEqual((void*)nullptr, buffer.data());
		Assert::AreEqual(BUFFER_SIZE, buffer.size()); 
	}

	TEST_METHOD(DeepCopyBuffer) {
		Buffer buffer(BUFFER_SIZE); 

		for (unsigned i = 0; i < BUFFER_SIZE / 4; ++i)
			buffer.data<int>()[i] = rand();
		 
		Buffer copy(buffer, true);
		 
		bool areEqual = true; 
		for (unsigned i = 0; i < BUFFER_SIZE / 4; ++i) {
			if (copy.data<int>()[i] != buffer.data<int>()[i]) {
				areEqual = false;
				break;
			}
		}

		Assert::AreEqual(copy.size(), buffer.size());
		Assert::AreNotEqual(copy.data(), buffer.data());
		Assert::IsTrue(areEqual); 
	}

	TEST_METHOD(CopyBuffer) {
		// Test if the buffers destructors are called in a safe manner when
		// a copy is made, which doesn't copy memory
		
		// Why did I make this a feature, when would we need this functionality?

		Buffer buffer(BUFFER_SIZE);  

		{
			Buffer copy(buffer, false);

			bool areEqual = true;
			for (unsigned i = 0; i < BUFFER_SIZE / 4; ++i) {
				if (copy.data<int>()[i] != buffer.data<int>()[i]) {
					areEqual = false;
					break;
				}
			}

			Assert::AreEqual(copy.size(), buffer.size());
			Assert::AreEqual(copy.data(), buffer.data()); 
		}

		Assert::AreNotEqual((void*)0, buffer.data()); 
	} 
};