#include "CGUID.h"

using namespace Pro;
using namespace Component;

CGUID::CGUID(const std::string& name){
	guid = GUIDLookup::getGUID(name);
}

CGUID::CGUID()
{
	guid = NULL;
}

CGUID::~CGUID()
{
}

int CGUID::lGetGUID(lua_State* L){
	CGUID* p = *static_cast<CGUID**>(lua_touserdata(L, 1));
	lua_pushnumber(L, p->getGUID());
	return 1;
}

unsigned int CGUID::getGUID(){
	return guid;
}