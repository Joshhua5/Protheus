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
	Position* p = *(static_cast<Position**>(lua_touserdata(L, 1)));
	Math::Vector2 position = p->getPosition();
	lua_pushnumber(L, position.x);
	lua_pushnumber(L, position.y);
	return 2;
}
int Position::lSetPosition(lua_State* L){
	Position* p = *(static_cast<Position**>(lua_touserdata(L, 1)));
	p->setPosition(Math::Vector2(
		static_cast<int>(lua_tonumber(L, 2)),
		static_cast<int>(lua_tonumber(L, 3))
		));
	return 0;
}
 