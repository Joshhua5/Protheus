#include <Queue.h>
#include <Log.h>
#include <thread>
#include <atomic>

using namespace Pro;
using namespace Util;

const unsigned count = 100000;

void creator(Queue<unsigned>* q, std::atomic<bool>* flag){
	while (flag->load());
	for (unsigned i = 0; i < count; ++i){
		while (q->size() == q->capacity() - 10);
		q->Push(i);
	}
}

void consumor(Queue<unsigned>* q, std::atomic<bool>* flag){
	while (flag->load()); 
	for (unsigned i = 0; i < count; ++i){
		while (q->Empty());
		if (q->Top() != i)
			global_log.Report<LogCode::MESSAGE>(std::to_string(q->Top()) + " expected " , nullptr, i); 
		q->Pop();
	}
}

int main() {
	Queue<unsigned> q(64);
	std::atomic<bool> go = true;

	std::thread(&creator, &q, &go).detach();
	std::thread(&consumor, &q, &go).detach();

	go.store(false);

	std::this_thread::sleep_for(std::chrono::seconds(60));
	global_log.Close();
	return 0;
}