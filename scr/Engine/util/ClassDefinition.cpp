#include "ClassDefinition.h"

using namespace Pro;
using namespace Serializer;
using namespace std;

ClassDefinition::ClassDefinition(void* class_pointer)
{
	base_pointer = class_pointer;
}

ClassDefinition::~ClassDefinition()
{
	delete base_pointer;
}

void ClassDefinition::addMember(const string& memberName, void* member_pointer, size_t size){
	auto m = Member();

	m.name = memberName;
	m.offset = static_cast<unsigned>((char*)member_pointer - (char*)base_pointer);
	m.size = static_cast<unsigned>(size);
	members.push_back(m);
}

vector<Member>& ClassDefinition::getMembers(){
	return members;
}