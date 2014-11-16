#include "Position.h"

using namespace Pro;
using namespace Component;
using namespace Math;
using namespace std;
 
Vector2 Position::getPosition() const{
	return position;
}


Vector2* Position::pGetPosition(){
	return &position;
}

void Position::setPosition(const Vector2& p){
	position = p;
}

void Position::setPosition(const Vector2&& p){
	position = p;
}

int Position::lGetPosition(lua_State* L){
	const auto p = Util::luaP_touserdata<Position>(L, 1);
	//Util::luaP_newobject<Vector2>(L, p->getPosition());
	return 1;
}

int Position::lSetPosition(lua_State* L){
	const auto p = Util::luaP_touserdata<Position>(L, 1);
	p->setPosition(Vector2(lua_tonumber(L, 2), lua_tonumber(L, 3)));
	return 0;
}