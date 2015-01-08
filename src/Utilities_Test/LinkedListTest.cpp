#include <Parallel.h>
#include <LinkedList.h>

using namespace Pro::Util;

const unsigned batch_size = static_cast<unsigned>(1E5);

void test(LinkedList<long>* list){
	list->append(new long(5));
	list->prepend(new long(1));
	for (unsigned x = 0; x < 5; ++x)
		list->prepend(new long(x));
	list->remove(0);

	// Possible nullptr and failed remove if using list->remove(list->size() - 1); 
	// as size might be updated between function call and execution 
	// Meaning the size will out of range and a nullptr will be given and therefor a nullptr exception
}

int main(){
	  
	LinkedList<long> list;
	Future future;
	 
	for (unsigned x = 0; x < batch_size; ++x)
		Parallel::batch(&test, nullptr, &list);
	Parallel::batch(&test, &future, &list);

	future.wait();

	return 0;
}

