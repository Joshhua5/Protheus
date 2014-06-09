/*************************************************************************
Protheus Source File.
Copyright (C), Protheus Studios, 2013-2014.
-------------------------------------------------------------------------

Description:

-------------------------------------------------------------------------
History:
- 17:05:2014: Waring J.
*************************************************************************/


#include "GUIDLookup.h"

namespace Pro{ 

	std::unordered_map<std::string, uint32>* GUIDLookup::getNameMapping(){
		static std::unordered_map<std::string, uint32> nameMapping;
		return &nameMapping;
	}

	void GUIDLookup::releaseGUID(uint32 i){
		getNameMapping()->erase(*getName(i)); 
	}

	void GUIDLookup::releaseGUID(const std::string& name){
		releaseGUID(getNameMapping()->at(name));
	}

	uint32 GUIDLookup::newGUID(const std::string& name){
		static uint32 allocatedBitCount = 1; 
		getNameMapping()->insert({ name, allocatedBitCount++ });
		return allocatedBitCount;
	}
	uint32 GUIDLookup::getGUID(const std::string& name){
		return getNameMapping()->at(name);
	}

	std::string* GUIDLookup::getName(uint32 _id){
		for each(auto elm in *getNameMapping())
			if (elm.second == _id)
				return &std::string(elm.first);
		return nullptr;
	}
}
 