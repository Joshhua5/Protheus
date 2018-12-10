#include "stdafx.h"
#include "CppUnitTest.h"


using namespace Pro;
using namespace Util;
using namespace Microsoft::VisualStudio::CppUnitTestFramework;


namespace UnitTests
{
	typedef std::chrono::time_point<std::chrono::high_resolution_clock> time_point;

	struct Object {
		mutable int x, y;
		Object() {
			x = 5; y = 3;
		}
		void add() {
			x += y;
		}
	};

	TEST_CLASS(PARALLEL_TEST) {

		static void call(void* argument) {
			++*static_cast<unsigned*>(argument);
			std::cout << "Finished";
		}

		static time_point now() {
			return std::chrono::high_resolution_clock::now();
		}

		static void counter(std::atomic<unsigned>* count) {
			count->fetch_add(1);
		}

		static void process(void* data, unsigned size, unsigned offset) {
			for (int x = offset; x < offset + size; ++x)
				static_cast<Object*>(data)[x].add();
		}

		static void pause() {
			std::this_thread::sleep_for(std::chrono::seconds(2));
		}

		static void nothing() { }

		TEST_METHOD(BATCH_TEST) {
			Sync sync;
			unsigned args = 0;
			Parallel& parallel = GetGlobalWorkPool();

			parallel.Batch(&PARALLEL_TEST::call, &sync, nullptr, 0, &args);
			Assert::IsTrue(sync.Wait(), L"", LINE_INFO());
		}

		TEST_METHOD(MEMBER_BATCH_TEST) {
			Sync sync;
			Parallel& parallel = GetGlobalWorkPool();

			Object object;
			parallel.MemberBatch(&Object::add, &sync, &object);

			sync.Wait();
			Assert::AreEqual(8, object.x, L"");
		}

		TEST_METHOD(STATIC_PROCESS_TEST) {
			Sync sync;
			Parallel& parallel = GetGlobalWorkPool();

			Object* object = new Object[41];
			Object* object1 = new Object[40];
			Object* object2 = new Object[2];

			parallel.Process(object, &Object::add, 41, 0, &sync);
			parallel.Process(object1, &Object::add, 40, 0, &sync);
			parallel.Process(object2, &Object::add, 2, 0, &sync);

			for (int x = 0; x < 41; ++x)
				Assert::AreEqual(8, object[x].x, L"");

			for (int x = 0; x < 40; ++x)
				Assert::AreEqual(8, object1[x].x, L"");

			for (int x = 0; x < 2; ++x)
				Assert::AreEqual(8, object2[x].x, L"");

			sync.Wait();

			delete[] object;
			delete[] object1;
			delete[] object2;
		}

		TEST_METHOD(SYNC_TEST) {
			Sync sync;
			Parallel& parallel = GetGlobalWorkPool();

			parallel.Batch(pause, &sync, nullptr, 0);

			Assert::IsFalse(sync.Wait(std::chrono::milliseconds(10)), L"The sync incorrectly waited for the job to finish");
			Assert::IsTrue(sync.Wait(std::chrono::seconds(10)), L"The sync never stopped waiting for the job to finish");
		}

		TEST_METHOD(DELAY_TEST) {
			Sync sync;
			Parallel& parallel = GetGlobalWorkPool();
			const int wait_time = 10000;

			parallel.Batch(nothing, &sync, nullptr, wait_time);
			auto start = GetGlobalTimer().GetTime<std::chrono::milliseconds>();
			sync.Wait(std::chrono::seconds(20));
			auto end = GetGlobalTimer().GetTime<std::chrono::milliseconds>();

			Assert::IsTrue(end - start > wait_time, L"Job before the specified delay");
		}

		TEST_METHOD(ADD_WORKER) {
			Parallel& parallel = GetGlobalWorkPool();
			auto start_size = parallel.WorkerCount();
			parallel.AddWorker(1);
			Assert::AreEqual(start_size + 1, parallel.WorkerCount(), L"Unable to produce another worker");
			parallel.KillWorker(1);
			// Wait so that we give time for a worker to pickup the kill command
			std::this_thread::sleep_for(std::chrono::seconds(1));
			Assert::AreEqual(start_size, parallel.WorkerCount(), L"Unable to kill a worker");
		}

		TEST_METHOD(LOAD_TEST) {
			Sync sync;
			Parallel& parallel = GetGlobalWorkPool();

			const unsigned load_size = 10000;
			std::atomic<unsigned> count = 0;

			for (unsigned x = 0; x < load_size; ++x)
				parallel.Batch(&PARALLEL_TEST::counter, nullptr, nullptr, 0, &count);

			sync.Wait();
			Assert::AreEqual(load_size, count.load(), L"Work was lost");
		}

		TEST_METHOD(THROUGHPUT_TEST) {
			Sync sync;
			Parallel& parallel = GetGlobalWorkPool();

			// Create 4 workers for every logical core
			unsigned original_count = parallel.WorkerCount();
			parallel.AddWorker((std::thread::hardware_concurrency() * 4) - parallel.WorkerCount());

			const unsigned load_size = std::thread::hardware_concurrency() * 4000;
			std::atomic<unsigned> count = 0;

			for (unsigned x = 0; x < load_size; ++x)
				parallel.Batch(&PARALLEL_TEST::counter, nullptr, nullptr, 0, &count);

			sync.Wait();
			Assert::AreEqual(load_size, count.load(), L"Work was lost");
			parallel.KillWorker(parallel.WorkerCount() - original_count);
		}
	};
}