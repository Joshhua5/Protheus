#include <Parallel.h> 
#include <iostream>

using namespace Pro;
using namespace Util;

struct Object {
	mutable float x, y;
	Object() {
		x = 5; y = 3;
	}
	void add(void*) {
		x += y;
	}
};

void call(void* argument) {
	++*static_cast<unsigned*>(argument);
	std::cout << "Finished";
}

int main() {
	Future finished;
	unsigned args = 0;
	std::function<void(void*)> func(&call);
	Parallel::batch(func, &finished, &args);
	
	finished.wait();

	Object* object = new Object[41];
	Object* object1 = new Object[40];
	Object* object2 = new Object[2];

	////Parallel::process(array*, func*, size, offset)
	
	Parallel::process<Object>(object, std::function<void(Object&, void*)>(&Object::add), 41, 0, nullptr, nullptr);
	Parallel::process<Object>(object1, std::function<void(Object&, void*)>(&Object::add), 40, 0, nullptr, nullptr);
	Parallel::process<Object>(object2, std::function<void(Object&, void*)>(&Object::add), 2, 0, &finished, nullptr);


	delete[] object;
	delete[] object1;
	delete[] object2;
	return 0;
}