#include "stdafx.h"
#include "CppUnitTest.h"
  
using namespace Microsoft::VisualStudio::CppUnitTestFramework;

using namespace Pro;
using namespace Util;

namespace Utilities_Test
{

	TEST_CLASS(QueueTest) {
		static const unsigned QUEUE_COUNT = 1000000;

		static void producer(Queue<unsigned>* q) {
			for (unsigned i = 0; i < QUEUE_COUNT; ++i) {
				q->Push(i);
			}
		}

		static void consumor(Queue<unsigned>* q, int* failed = nullptr) {
			for (unsigned i = 0; i < QUEUE_COUNT; ++i) {
				unsigned value;
				q->Top(value);
				if (value != i && failed != nullptr)
					*failed = i; 
				q->Pop();
			}
		}

		TEST_METHOD(Creation) { 
			Queue<unsigned> q(QUEUE_COUNT);
			
			Assert::AreEqual(QUEUE_COUNT, (unsigned)q.capacity());
		}

		TEST_METHOD(Resize) {
			Queue<unsigned> queue(64);

			for (unsigned i = 0; i < 32; ++i)
				queue.Push(i);
			queue.Resize(128);
			for (unsigned i = 32; i < 64; ++i)
				queue.Push(i);

			unsigned value = 0;
			for (unsigned i = 0; i < 64; ++i)
			{
				queue.TopPop(value);  
				Assert::AreEqual(i, value);  
			}
		}

		TEST_METHOD(Circulation) {
			Queue<unsigned> queue(32);

			// Position the array over the circular buffer
			for (unsigned i = 0; i < 24; ++i)
				queue.Push(i);  
			for (unsigned i = 0; i < 24; ++i)
				queue.Pop();

			// Push so we loop around
			for (unsigned i = 0; i < 16; ++i)
				queue.Push(i);

			// Check the values
			unsigned value = 0;
			for (unsigned i = 0; i < 16; ++i)
			{
				queue.TopPop(value);
				Assert::AreEqual(i, value);
			}
		}

		//! The queue is a circular buffer, therefor we should be able to resize
		//! while the data is split between the start and end of the array
		TEST_METHOD(CirculatedResize) {
			Queue<unsigned> queue(32);

			// Position the array over the circular buffer
			for (unsigned i = 0; i < 24; ++i)
				queue.Push(i);
			for (unsigned i = 0; i < 24; ++i)
				queue.Pop();

			// Push so we loop around
			for (unsigned i = 0; i < 16; ++i)
				queue.Push(i);

			// We have values over the border of the circular buffer
			// Lets resize
			queue.Resize(64); 

			// Check the values
			unsigned value = 0;
			for (unsigned i = 0; i < 16; ++i)
			{
				queue.TopPop(value);   
				Assert::AreEqual(i, value);
			} 
		}

		TEST_METHOD(SingleConsumerProducer) { 
			Queue<unsigned> q(QUEUE_COUNT);

			int consumer_validation_fail = 0;

			producer(&q);
			consumor(&q, &consumer_validation_fail);

			Assert::AreEqual(0, consumer_validation_fail);
		} 

		TEST_METHOD(QueueCapacityOverflow) {
			unsigned long long time = 0;
			Queue<unsigned> q(QUEUE_COUNT / 10);
			producer(&q);
			consumor(&q);
		}

		TEST_METHOD(OrderPreservation) {
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
					Assert::AreEqual(x, value,
						(L"Unexpected value returned:" + std::to_wstring(x) + L", expected " + std::to_wstring(value)).data(),
						LINE_INFO());
				}
			}
		}
	}; 
}