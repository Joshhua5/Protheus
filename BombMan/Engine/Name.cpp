#include "Name.h"
namespace Pro{

	void Name::setName(const std::string& _name){
		name = _name;
	}

	const std::string Name::getName(){
		return name;
	}
}