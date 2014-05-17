#include "Volume.h"

using namespace Pro;

Volume::Volume()
{
}


Volume::~Volume()
{
}
 
Math::Vector2 Volume::getDimensions(){
	return dimensions;
}
Math::Vector2* Volume::pGetDimensions(){
	return &dimensions;
}
void Volume::setDimensions(Math::Vector2 v){
	dimensions = v;
}
  
int Volume::lGetDimensions(lua_State* L){
	Volume* p = *(static_cast<Volume**>(lua_touserdata(L, 1)));
	Math::Vector2* dim = p->pGetDimensions();
	lua_pushnumber(L, dim->x);
	lua_pushnumber(L, dim->y);
	return 2;
}
int Volume::lSetDimensions(lua_State* L){
	Volume* p = *(static_cast<Volume**>(lua_touserdata(L, 1)));
	p->setDimensions(Math::Vector2(
		static_cast<int>(lua_tonumber(L, 2)),
		static_cast<int>(lua_tonumber(L, 3))
		));
	return 0;
}