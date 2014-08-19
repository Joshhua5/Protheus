#include "GUIEntity.h"

using namespace Pro;
using namespace GUI;
using namespace Util;
using namespace Math;

GUIEntity::GUIEntity(const std::string& name) : CGUID(name), LuaCallback(){}
GUIEntity::GUIEntity() : CGUID(), LuaCallback(){}

GUIEntity* GUIEntity::getParent() const{
	return parent;
}
void GUIEntity::setParent(GUIEntity* _parent){
	parent = _parent;
}

bool GUIEntity::isClickWithin(const Vector2& v) const{
	return Vector4(position, dimensions).contains(v);
}

int GUIEntity::lGetParent(lua_State* L){
	const auto p = luaP_touserdata<GUIEntity>(L, 1);
	luaP_newobject<GUIEntity>(L, p->getParent()); 
	return 1;
}
int  GUIEntity::lSetParent(lua_State* L){
	const auto p = luaP_touserdata<GUIEntity>(L, 1);
	p->setParent(luaP_touserdata<GUIEntity>(L, 2));
	return 0;
}

int GUIEntity::lIsClickWithin(lua_State* L){
	const auto p = Util::luaP_touserdata<GUIEntity>(L, 1); 
	lua_pushboolean(L, p->isClickWithin(*Util::luaP_touserdata<Vector2>(L, 2)));
	return true;
}