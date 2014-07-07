
#include "CGUID.h"

using namespace Pro;
using namespace Component;

CGUID::CGUID(const std::string& name){
	guid = GUIDLookup::newGUID(name);
}

CGUID::CGUID()
{
	guid = GUIDLookup::newGUID("");
}

CGUID::~CGUID()
{
	GUIDLookup::releaseGUID(guid);
}

int CGUID::lGetGUID(lua_State* L){
	const auto p = *static_cast<CGUID**>(lua_touserdata(L, 1));
	lua_pushnumber(L, p->getGUID());
	return 1;
}

game_id CGUID::getGUID(){
	return guid;
}