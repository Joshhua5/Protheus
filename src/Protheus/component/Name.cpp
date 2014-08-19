#include "Name.h"
using namespace Pro;
using namespace Component;

void Name::setName(const std::string& _name){
	name = _name;
}

const std::string Name::getName(){
	return name;
}