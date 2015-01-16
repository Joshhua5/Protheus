#include <Parallel.h>
#include "gtest\gtest.h"
#include "TestCls.h"

using namespace Pro::Util;

#define BATCH_TEST_COUNT 100

void testFunction(std::atomic<unsigned>* ptr) {
	++*ptr;
}

TEST(Parallel_Test, Batch_Test) {
	reset_counters();
	std::atomic<unsigned> atom = 0;
	Future future;
	for (unsigned x = 0; x < BATCH_TEST_COUNT; ++x) {
		parallel.batch(testFunction, nullptr, &atom);
	}
	parallel.batch(testFunction, &future, &atom);
	future.wait();

	ASSERT_EQ(atom.load(), BATCH_TEST_COUNT + 1);
}

TEST(Parallel_Test, Process_Object_Test_Equal) {
	reset_counters();
	TestCls* values = new TestCls[DEFAULT_PARALLEL_THREAD_COUNT];
	for (unsigned x = 0; x < DEFAULT_PARALLEL_THREAD_COUNT; ++x)
		values[x] = x;

	Future future;
	parallel.process(values, &TestCls::operator*=, DEFAULT_PARALLEL_THREAD_COUNT, 0, &future, 2);
	future.wait();

	ASSERT_EQ(DEFAULT_PARALLEL_THREAD_COUNT, multiplication_assignment);
	for (unsigned x = 0; x < DEFAULT_PARALLEL_THREAD_COUNT; ++x)
		ASSERT_EQ(x * 2, values[x].value());
	delete[] values;
}

TEST(Parallel_Test, Process_Object_Test_Small) {
	reset_counters();
	TestCls* values = new TestCls[DEFAULT_PARALLEL_THREAD_COUNT - 1];
	for (unsigned x = 0; x < DEFAULT_PARALLEL_THREAD_COUNT - 1; ++x)
		values[x] = x;

	Future future;
	parallel.process(values, &TestCls::operator*=, DEFAULT_PARALLEL_THREAD_COUNT - 1, 0, &future, 2);
	future.wait();

	for (unsigned x = 0; x < DEFAULT_PARALLEL_THREAD_COUNT - 1; ++x)
		ASSERT_EQ(x * 2, values[x].value());

	delete[] values;
}

TEST(Parallel_Test, Process_Object_Test_Large) {
	reset_counters();
	TestCls* values = new TestCls[DEFAULT_PARALLEL_THREAD_COUNT + 1];
	for (unsigned x = 0; x < DEFAULT_PARALLEL_THREAD_COUNT + 1; ++x)
		values[x] = x;

	Future future;
	parallel.process(values, &TestCls::operator*=, DEFAULT_PARALLEL_THREAD_COUNT + 1, 0, &future, 2);
	future.wait();

	for (unsigned x = 0; x < DEFAULT_PARALLEL_THREAD_COUNT + 1; ++x)
		ASSERT_EQ(x * 2, values[x].value());
	delete[] values;
}

TEST(Parallel_Test, Future_Test) {
	reset_counters();
	TestCls* values = new TestCls[DEFAULT_PARALLEL_THREAD_COUNT];
	for (unsigned x = 0; x < DEFAULT_PARALLEL_THREAD_COUNT; ++x)
		values[x] = x;

	Future future;
	parallel.process(values, &TestCls::operator*=, DEFAULT_PARALLEL_THREAD_COUNT, 0, &future, 2);

	ASSERT_EQ(DEFAULT_PARALLEL_THREAD_COUNT, future.worker_count);
	future.wait();
	ASSERT_EQ(future.finished_count, future.worker_count);
	delete[] values;
}