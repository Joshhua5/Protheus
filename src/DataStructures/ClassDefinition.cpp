#include "ClassDefinition.h"

using namespace Pro;
using namespace Serializer;
using namespace std;

ClassDefinition::ClassDefinition(void* class_pointer)
{
	base_pointer = class_pointer;
}
 
void ClassDefinition::addMember(const string& memberName,const void* member_pointer,const size_t size){
	auto m = Member();

	m.name = memberName;
	m.offset = static_cast<unsigned>((char*)member_pointer - (char*)base_pointer);
	m.size = static_cast<unsigned>(size);

	members.push_back(move(m));
}

const vector<Member>& ClassDefinition::getMembers() const{
	return members;
}

const unsigned ClassDefinition::getSizeOf() const{
	unsigned sizeTotal = 0;
	for each(const auto m in members)
		sizeTotal += m.size;
	return sizeTotal;
}

const void* ClassDefinition::getBase() const{
	return base_pointer;
} 

void ClassDefinition::finish(){
	delete base_pointer;
}