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
	static unordered_map<std::string, game_id> nameMapping;
	return &nameMapping;
}

void GUIDLookup::releaseGUID(const game_id i){
	getNameMapping()->erase(getName(i));
}

void GUIDLookup::releaseGUID(const std::string& name){
	releaseGUID(getNameMapping()->at(name));
}

game_id GUIDLookup::newGUID(const std::string& name){
	static auto allocatedBitCount = 1;
	getNameMapping()->insert({ name, allocatedBitCount++ });
	return allocatedBitCount;
}
game_id GUIDLookup::getGUID(const std::string& name){
	return getNameMapping()->at(name);
}

const string& GUIDLookup::getName(const game_id _id){
	for each(const auto elm in *getNameMapping())
		if (elm.second == _id)
			return elm.first;
	return nullptr;
}

static int lGetGUID(lua_State* L){
	lua_pushnumber(
		L,
		GUIDLookup::getGUID(lua_tostring(L, 1))
		);
	return 1;
}
static int lGetName(lua_State* L){
	lua_pushstring(
		L,
		GUIDLookup::getName(static_cast<game_id>(lua_tonumber(L, 1))).c_str());
	return 1;
}