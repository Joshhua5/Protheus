#include "Area.h"

using namespace Pro;
using namespace Component;
Area::Area()
{
}


Area::~Area()
{
}
 
Math::Vector2 Area::getDimensions(){
	return dimensions;
}
Math::Vector2* Area::pGetDimensions(){
	return &dimensions;
}
void Area::setDimensions(Math::Vector2& v){
	dimensions = v;
}
  
int Area::lGetDimensions(lua_State* L){
	Area* p = *(static_cast<Area**>(lua_touserdata(L, 1)));
	Math::Vector2* dim = p->pGetDimensions();
	lua_pushnumber(L, dim->x);
	lua_pushnumber(L, dim->y);
	return 2;
}
int Area::lSetDimensions(lua_State* L){
	Area* p = *(static_cast<Area**>(lua_touserdata(L, 1)));
	p->setDimensions(Math::Vector2(
		static_cast<int>(lua_tonumber(L, 2)),
		static_cast<int>(lua_tonumber(L, 3))
		));
	return 0;
}