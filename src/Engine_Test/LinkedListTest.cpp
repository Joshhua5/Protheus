#include "stdafx.h"
#include "CppUnitTest.h"


using namespace Pro;
using namespace Util;

const unsigned batch_size = static_cast<unsigned>(1E5);
const unsigned thread_count = 4;

std::atomic<bool> start(false);
std::atomic<char> finish_count(0);
std::atomic<unsigned> result(0);
std::atomic<unsigned> initial_size(0);
std::atomic<unsigned> final_size(0);

void test_worker(LinkedList<long>* list) {
	unsigned push_size = 0;
	unsigned pop_size = 0;
	while (start.load() == false);
	for (long x = 0; x < batch_size; ++x) {
		list->PushBack(x);
		push_size += x;
	}  
	finish_count.fetch_add(1);

	while (finish_count.load() != thread_count);

	for (long x = 0; x < batch_size; ++x)
		pop_size += list->PopBack(); 

	initial_size.fetch_add(push_size);
	final_size.fetch_add(pop_size); 
}


int main(){ 
	char exit_status = 0;
	global_log.EchoOnConsole(true);
	{
		LinkedList<long> list;

		list.PushBack(5);
		list.PushFront(1);
		for (long x = 0; x < 5; ++x)
			list.PushBack(x);
		list.Remove(0);
		if (list.size() != 6){
			global_log.Report<LogCode::FAULT>("Incorrect final size: " + std::to_string(list.size()), __FUNCTION__, __LINE__);
			exit_status |= 1;
		}
	} 
	{ 
		LinkedList<long> list;

		std::thread threads[thread_count];
		for (unsigned x = 0; x < thread_count; ++x)
			threads[x] = std::thread(test_worker, &list);
		start = true;
		for (unsigned x = 0; x < thread_count; ++x)
			threads[x].join();

		if (final_size != initial_size){
			global_log.Report<LogCode::FAULT>("Incorrect final size, values were lost in the list" + std::to_string(initial_size.load()) + " " + std::to_string(final_size.load()),
				__FUNCTION__, __LINE__);
			exit_status |= 2;
		}
	}
	 
	return exit_status;
}

