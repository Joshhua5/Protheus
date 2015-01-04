
#include <ArrayList.h>

using namespace Pro::Util;

void copyArr(const ArrayList<unsigned>& lst) {
	return;
}

void moveArr(ArrayList<unsigned>&& lst) {
	lst.push_back(56);
}

void valueArr(ArrayList<unsigned> lst) {
	lst.push_back(5);
}

void selfDeletion() {
	ArrayList<unsigned> lst;
	lst.push_back(5);
	lst.push_back(2);
	++lst.at(0);
}

void advDeletion() {
	ArrayList<ArrayList<unsigned>> lst;
	lst.push_back(ArrayList<unsigned>());
	lst.at(0).push_back(5); 
}

int main() { 
	ArrayList<unsigned> list;
	list.push_back(4);
	copyArr(list);
	valueArr(list);
	moveArr(std::move(list));

	selfDeletion();
	advDeletion();

	return 0;
}