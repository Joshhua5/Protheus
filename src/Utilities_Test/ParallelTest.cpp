#include <Parallel.h>
#include <Timer.h>
#include <iostream>
#ifdef  WIN32 || (_WIN32 ) || defined ( _WIN64 ) || defined(__WIN32__)
#include <windows.h>
#include <vector>
#endif

using namespace Pro;
using namespace Util;

struct Object {
	mutable float x, y;
	Object() {
		x = 5; y = 3;
	}
	void add() {
		x += y;
	}
};

void call(void* argument) {
	++*static_cast<unsigned*>(argument);
	std::cout << "Finished";
}
#ifdef WIN32 || defined ( _WIN32 ) || defined ( _WIN64 ) || defined(__WIN32__)
void latency(LARGE_INTEGER* tick2) {
	QueryPerformanceCounter(tick2);
}
#endif

int main() {
	Future finished;
	unsigned args = 0;
	Parallel::batch(&call, &finished, &args);

	finished.wait();

	Object* object = new Object[41];
	Object* object1 = new Object[40];
	Object* object2 = new Object[2];

	////Parallel::process(array*, func*, size, offset)
	Parallel::process(object, &Object::add, 41, 0, nullptr);
	Parallel::process(object1, &Object::add, 40, 0, nullptr);
	Parallel::process(object2, &Object::add, 2, 0, nullptr);
	// Test the batch latency

	while (!Parallel::isQueueEmpty()) {}

#ifdef WIN32 || defined ( _WIN32 ) || defined ( _WIN64 ) || defined(__WIN32__)
	std::vector<double> time;
	LARGE_INTEGER CPS;
	QueryPerformanceFrequency(&CPS);

	LARGE_INTEGER tick1, tick2;

	for (unsigned x = 0; x < 10000; ++x) {
		QueryPerformanceCounter(&tick1);
		Parallel::batch(&latency, &finished, &tick2);
		finished.wait();
		time.push_back((tick2.QuadPart - tick1.QuadPart) / (double(CPS.QuadPart) / 1000000.0));
	}

	// Average

	double avg = 0;
	double min = 10000000.0;
	double max = 0;
	for (auto& x : time) {
		avg += x;
		if (x < min)
			min = x;
		else if (x > max)
			max = x;
	}
	avg /= time.size();

	std::cout << "Thread Batch Delay Average(ms): " << avg << std::endl;
	std::cout << "Thread Batch Delay Min(ms): " << min << std::endl;
	std::cout << "Thread Batch Delay Max(ms): " << max << std::endl;
	system("pause");
#endif	  

	delete[] object;
	delete[] object1;
	delete[] object2;

	return 0;
}