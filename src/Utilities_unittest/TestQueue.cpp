
#include <Queue.h>
#include "gtest\gtest.h"
#include <thread>
#include "TestCls.h"
#include <condition_variable>
#include <mutex>
#include <vector>

using namespace Pro::Util;

const unsigned TEST_SIZE = 1000;
const unsigned last = 2014;
const unsigned THREAD_COUNT = 10;

std::mutex mut;
std::condition_variable cv;

void push(Queue<unsigned>* queue, size_t count, size_t offset) {
	cv.wait(std::unique_lock<std::mutex>(mut));  
	for (unsigned x = offset; x < offset + count; ++x)
		queue->push(x);
}
 
void pop(Queue<unsigned>* queue, size_t count, size_t offset, std::atomic<unsigned>* result) {
	cv.wait(std::unique_lock<std::mutex>(mut));
	unsigned total = 0;
	for (unsigned x = offset; x < offset + count; ++x)
		total += queue->pop();
}
 
TEST(Queue_Test, Single_Thread_Push_Pop_Test) {
	Queue<unsigned> queue;
	unsigned long total = 0;
	unsigned long result = 0;
	
	for (unsigned x = 0; x < TEST_SIZE; ++x) {
		result += x;
		queue.push(x);
	}
	  
	ASSERT_EQ(TEST_SIZE, queue.size());

	for (unsigned x = 0; x < TEST_SIZE; ++x)
		total += queue.pop();
	  
	ASSERT_EQ(0, queue.size());
	ASSERT_EQ(result, total);
}  

TEST(Queue_Test, Resize_Test) {
	Queue<unsigned> q;
	  
	// Value push is after pop
	for (unsigned x = 0; x < 32; ++x)
		q.push(x);

	q.resize(96);

	for (unsigned x = 0; x < 16; ++x)
		q.pop();

	// Value push has overlapped once

	for (unsigned x = 0; x < 86; ++x)
		q.push(x);
	  
	ASSERT_EQ(q.size(), 102); 

	// Test that the array can be resized to a smaller volume.

	while (q.size() != 10)
		q.pop();  

	ASSERT_NO_THROW(q.resize(15));

	// Test resizing to a smaller value

	ASSERT_NO_THROW(q.resize(9));
	ASSERT_EQ(q.capacity(), 15); 
}

TEST(Queue_Test, Single_Thread_Consistency_Test) {
	Queue<unsigned> q;

	for (unsigned x = 0; x < TEST_SIZE; ++x)
		q.push(x);

	for (unsigned x = 0; x < TEST_SIZE; ++x)
		ASSERT_EQ(x, q.pop());
}
 

//TEST(Queue_Test, Multi_Thread_Push_Pop_Test) {
//	Queue<unsigned> queue(10000);
//	std::vector<std::thread> threads; 
//	unsigned long result = (TEST_SIZE * (TEST_SIZE + 1)) / 2;
//	
//	queue.push(last);
//
//	for (unsigned x = 0; x < THREAD_COUNT; ++x)
//		threads.push_back(std::thread(&push, &queue, TEST_SIZE / THREAD_COUNT, (TEST_SIZE / THREAD_COUNT) * x));
//	cv.notify_all();
//	 
//	// Wait for finish
//	for (auto& x : threads)
//		if(x.joinable())
//			x.join();
//
//	std::atomic<unsigned> result_a;
//
//	for (unsigned x = 0; x < THREAD_COUNT; ++x)
//		threads.push_back(std::thread(&pop, &queue, TEST_SIZE / THREAD_COUNT, (TEST_SIZE / THREAD_COUNT) * x, &result_a));
//
//	cv.notify_all();
//	// Wait for finish
//	for (auto& x : threads)
//		x.join();
//
//	ASSERT_EQ(TEST_SIZE + 1, queue.size());
//	  
//	unsigned last_result = queue.pop();
//
//	ASSERT_EQ(last, last_result);
//	ASSERT_EQ(0, queue.size());
//	ASSERT_EQ(result, result_a);
//}

TEST(Queue_Test, Object_Push_Pop_Test) {
	Queue<TestCls> queue;

	queue.push(TestCls(2));

	queue.push(TestCls(1));

	ASSERT_EQ(queue.pop().value(), 2);
	ASSERT_EQ(queue.pop().value(), 1);
} 