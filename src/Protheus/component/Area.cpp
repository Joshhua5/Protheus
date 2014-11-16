#include "Area.h"

using namespace Pro;
using namespace Component;
using namespace Math;

Area::Area(const Vector2& dim){
	dimensions = dim;
} 

Vector2 Area::getDimensions() const{
	return dimensions;
}
Vector2* Area::pGetDimensions() {
	return &dimensions;
}
void Area::setDimensions(const Vector2& v){
	dimensions = v;
}

void Area::setDimensions(const Vector2&& v){
	dimensions = v;
}

float Area::getVolume() const{
	return dimensions.x * dimensions.y;
}

int Area::lGetDimensions(lua_State* L){
	const auto p = Util::luaP_touserdata<Area>(L, 1);
	//Util::luaP_newobject<Vector2>(L, p->pGetDimensions());
	return 1;
}
int Area::lSetDimensions(lua_State* L){
	const auto p = *(static_cast<Area**>(lua_touserdata(L, 1)));
	p->setDimensions(Vector2(lua_tonumber(L, 2), lua_tonumber(L, 3)));
	return 0;
}