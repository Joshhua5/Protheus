#include "stdafx.h"
#include "CppUnitTest.h"

#define LOGNAME "QueueTest.xml"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

using namespace Pro;
using namespace Util;
 

TEST_CLASS(QUEUE_TEST) {
	const unsigned QUEUE_COUNT = 1000000; 

	void creator(Queue<unsigned>* q) {
		for (unsigned i = 0; i < QUEUE_COUNT; ++i) {
			q->Push(i);
		}
	}

	void consumor(Queue<unsigned>* q) {
		for (unsigned i = 0; i < QUEUE_COUNT; ++i) {
			unsigned value;
			q->Top(value);
			if (value != i)
				Assert::IsTrue(false, (std::to_wstring(value) + L" expected " + std::to_wstring(i)).data(), LINE_INFO()); 
			q->Pop();
		}
	}


	TEST_METHOD(QUEUE_WITH_CAPACITY) { 
		unsigned long long time = 0;
		Queue<unsigned> q(QUEUE_COUNT);
		creator(&q);
		consumor(&q); 
	}

	TEST_METHOD(QUEUE_WITHOUT_CAPACITY) { 
		unsigned long long time = 0;
		Queue<unsigned> q(QUEUE_COUNT / 10);
		creator(&q);
		consumor(&q); 
	}

	TEST_METHOD(EXPECTED_ORDER_TEST) { 
		Queue<unsigned> values;
		for (unsigned x = 0; x < 63; ++x) {
			values.Push(x);
		}

		for (unsigned x = 0; x < 10; ++x) {
			unsigned value;
			values.TopPop(value);
		}

		values.Resize(128);

		for (unsigned x = 0; x < 5; ++x)
			values.Push(x);

		for (unsigned x = 10; x < 20; ++x) {
			unsigned value;
			values.TopPop(value);
			if (x != value) {
				Assert::IsTrue(x == value,
					(L"Unexpected value returned:" + std::to_wstring(x) + L", expected " + std::to_wstring(value)).data(),
					LINE_INFO()); 
			}
		}
	}  
};


