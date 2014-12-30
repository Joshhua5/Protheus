#include <Parallel.h>
#include <iostream>

using namespace Pro;
using namespace Util;

struct Object {
	float x, y;
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
	bool finished;
	unsigned args = 0;

	Parallel::batch(&call, &args, nullptr);
	Parallel::batch(&call, &args, nullptr); 
	Parallel::batch(&call, &args, nullptr);
	Parallel::batch(&call, &args, nullptr); 
	Parallel::batch(&call, &args, nullptr); 
	Parallel::batch(&call, &args, &finished);
	 
	Object* object = new Object[50];

	//Parallel::process(array*, func*, size, offset)
	Parallel::process<Object>(object, &Object::add, 50, 0);

	while (!finished) { } 

	delete[] object;
	return 0;
}