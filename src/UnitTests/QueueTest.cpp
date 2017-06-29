#include "stdafx.h"
#include "CppUnitTest.h"

#define LOGNAME "QueueTest.xml"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

using namespace Pro;
using namespace Util;
 
const unsigned count = 1000000; 

TEST_CLASS(QUEUE_TEST) {

	void creator(Queue<unsigned>* q) {
		for (unsigned i = 0; i < count; ++i) {
			q->Push(i);
		}
	}

	void consumor(Queue<unsigned>* q) {
		for (unsigned i = 0; i < count; ++i) {
			unsigned value;
			q->Top(value);
			if (value != i)
				Assert::IsTrue(false, (std::to_wstring(value) + L" expected " + std::to_wstring(i)).data(), LINE_INFO()); 
			q->Pop();
		}
	}


	TEST_METHOD(QUEUE_WITH_CAPACITY) { 
		unsigned long long time = 0;
		Queue<unsigned> q(count); 
		creator(&q);
		consumor(&q); 
	}

	TEST_METHOD(QUEUE_WITHOUT_CAPACITY) { 
		unsigned long long time = 0;
		Queue<unsigned> q(count / 10);  
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


