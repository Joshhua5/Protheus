#include "CGUID.h"

using namespace Pro;
using namespace Component;

CGUID::CGUID(const std::string& _name){
	guid = GUIDLookup::newGUID(_name);
	name = _name;
}

CGUID::CGUID()
{
	guid = GUIDLookup::newGUID("");
	name = string("");
}

CGUID::~CGUID()
{
	GUIDLookup::releaseGUID(name);
}

int CGUID::lGetGUID(lua_State* L){
	const auto p = *static_cast<CGUID**>(lua_touserdata(L, 1));
	lua_pushnumber(L, p->getGUID());
	return 1;
}

game_id CGUID::getGUID(){
	return guid;
}

string CGUID::getName(){
	return name;
}