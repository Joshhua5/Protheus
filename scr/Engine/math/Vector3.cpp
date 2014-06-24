
#include "Vector3.h"

using namespace Pro;
using namespace Math;

Vector3::Vector3()
{
	x = y = z = 0;
}


Vector3::~Vector3()
{
} 

int Vector3::lGetX(lua_State* L){
	Vector3* v = Util::luaP_touserdata<Vector3>(L, 1);
	lua_pushnumber(L, v->x);
	return 1;
}
int Vector3::lGetY(lua_State* L){
	Vector3* v = Util::luaP_touserdata<Vector3>(L, 1);
	lua_pushnumber(L, v->y);
	return 1;
}
int Vector3::lGetZ(lua_State* L){
	Vector3* v = Util::luaP_touserdata<Vector3>(L, 1);
	lua_pushnumber(L, v->z);
	return 1;
}
int Vector3::lSetX(lua_State* L){
	Vector3* v = Util::luaP_touserdata<Vector3>(L, 1);
	v->x = lua_tonumber(L, 2); 
	return 0;
}
int Vector3::lSetY(lua_State* L){
	Vector3* v = Util::luaP_touserdata<Vector3>(L, 1);
	v->y = lua_tonumber(L, 2);
	return 0;
}
int Vector3::lSetZ(lua_State* L){
	Vector3* v = Util::luaP_touserdata<Vector3>(L, 1);
	v->z = lua_tonumber(L, 2);
	return 0;
}
int Vector3::lGetXYZ(lua_State* L){
	Vector3* v = Util::luaP_touserdata<Vector3>(L, 1);
	float a[] = { v->x, v->y, v->z };
	Util::luaP_pusharray(L, a, 3);
	return 1;
}
int Vector3::lSetXYZ(lua_State* L){
	Vector3* v = Util::luaP_touserdata<Vector3>(L, 1);
	v->x = lua_tonumber(L, 1);
	v->y = lua_tonumber(L, 2);
	v->z = lua_tonumber(L, 3);
	return 0;
}