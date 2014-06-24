
#include "Position.h"

using namespace Pro;
using namespace Component;

Position::Position()
{
}


Position::~Position()
{
}

Math::Vector2 Position::getPosition(){
	return position;
}
Math::Vector2* Position::pGetPosition(){
	return &position;
}
void Position::setPosition(Math::Vector2& p){
	position = p;
}
  
int Position::lGetPosition(lua_State* L){
	auto p = Util::luaP_touserdata<Position>(L, 1);
	auto v = p->getPosition();
	lua_pushnumber(L, v.x);
	lua_pushnumber(L, v.y);
	return 0;
}

int Position::lSetPosition(lua_State* L){
	auto p = Util::luaP_touserdata<Position>(L, 1);
	auto v = Math::Vector2(static_cast<int>(lua_tonumber(L, 2)),
		static_cast<int>(lua_tonumber(L, 3)));
	p->setPosition(v);
	return 0;
}