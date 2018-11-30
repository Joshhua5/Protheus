#include "stdafx.h"
#include "CppUnitTest.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

using namespace Pro;
using namespace Util;

#include "stdafx.h"
#include "CppUnitTest.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

using namespace Pro;
using namespace Util;

namespace Utilities_Test
{ 
	TEST_CLASS(PipeTest) {
		static const unsigned QUEUE_COUNT = 1000000;

		static void producer(Pipe<unsigned>* q) {
			for (unsigned i = 0; i < QUEUE_COUNT; ++i) {
				q->Push(i);
			}
		}

		static void consumor(Pipe<unsigned>* q, int* failed = nullptr) {
			for (unsigned i = 0; i < QUEUE_COUNT; ++i) {
				unsigned value;
				q->Top(value);
				if (value != i && failed != nullptr)
					*failed = i;
				q->Pop();
			}
		}

		TEST_METHOD(Creation) {
			Pipe<unsigned> q(QUEUE_COUNT);

			Assert::AreEqual(QUEUE_COUNT, (unsigned)q.capacity());
		} 

		TEST_METHOD(Circulation) {
			Pipe<unsigned> queue(32);

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

		TEST_METHOD(SingleConsumerProducer) {
			Pipe<unsigned> q(QUEUE_COUNT);

			int consumer_validation_fail = 0;

			producer(&q);
			consumor(&q, &consumer_validation_fail);

			Assert::AreEqual(0, consumer_validation_fail);
		}

		// TODO replace this with a time based test, where they produce and consume as much as possible
		// within 5 seconds.
		TEST_METHOD(MultiConsumerProducer) {
			Pipe<unsigned> q(QUEUE_COUNT + 1);

			// 0 for success, otherwise it'll be the index of the fail
			int consumer_validation_fail = 0;
			 
			std::thread creator(&PipeTest::producer, &q);
			std::thread consumer(&PipeTest::consumor, &q, &consumer_validation_fail);

			creator.join();
			consumer.join();

			Assert::AreEqual(0, consumer_validation_fail);
		}

		TEST_METHOD(QueueCapacityOverflow) {
			unsigned long long time = 0;
			Pipe<unsigned> q(QUEUE_COUNT / 10);
			producer(&q);
			consumor(&q);
		}

		TEST_METHOD(OrderPreservation) {
			Pipe<unsigned> values(128);
			for (unsigned x = 0; x < 63; ++x) {
				values.Push(x);
			}

			for (unsigned x = 0; x < 10; ++x) {
				unsigned value;
				values.TopPop(value);
			} 

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