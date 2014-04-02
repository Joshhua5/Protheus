#include "IDManager.h"

namespace Pro{

	IDManager::IDManager()
	{
	}


	IDManager::~IDManager()
	{
	}

	ID IDManager::getID(){
		if (releasedIDs.size() == 0){
			allocatedBitCount++;
			return allocatedBitCount - 1;
		}
		else{
			ID id = releasedIDs.top();
			releasedIDs.pop();
			return id;
		}
	}

	void IDManager::releaseID(ID _id){
		releasedIDs.push(_id);
	}

	void IDManager::releaseID(const std::string& name){
		releaseID(nameMapping.at(name));
	}

	ID IDManager::getID(const std::string& name){
		ID id = getID();
		nameMapping.insert({ name, id });
		return id;
	}
	ID IDManager::getIDFromName(const std::string& name){
		return nameMapping.at(name);
	}

}