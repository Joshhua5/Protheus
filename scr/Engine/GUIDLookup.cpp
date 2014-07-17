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

using namespace Pro;
using namespace std;

unordered_map<string, game_id>* GUIDLookup::getNameMapping(){
	static unordered_map<string, game_id> nameMapping;
	return &nameMapping;
}

void GUIDLookup::releaseGUID(const game_id i){
	getNameMapping()->erase(getName(i));
}

void GUIDLookup::releaseGUID(const string& name){
	getNameMapping()->erase(name);
} 

game_id GUIDLookup::newGUID(const std::string& name){
	static game_id allocatedBitCount = 1;
	getNameMapping()->insert({ name, allocatedBitCount });
	return allocatedBitCount++;
}
game_id GUIDLookup::getGUID(const std::string& name){
	static const auto map = getNameMapping();
	
#ifdef DEBUG
	const auto got = map->find(name);
	return (got == map->end()) ? 0 : got->second;

#else
	return getNameMapping()->at(name);
#endif 
}

const string GUIDLookup::getName(const game_id _id){ 
	static const auto map = getNameMapping();
	for each(const auto elm in *map)
		if (elm.second == _id)
			return elm.first;
	return "";
}

int GUIDLookup::lGetGUID(lua_State* L){
	lua_pushnumber(L, GUIDLookup::getGUID(lua_tostring(L, 1)));
	return 1;
}
int GUIDLookup::lGetName(lua_State* L){
	lua_pushstring(L, GUIDLookup::getName(static_cast<game_id>(lua_tonumber(L, 1))).data());
	return 1;
}