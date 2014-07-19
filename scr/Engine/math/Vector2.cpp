#include "Vector2.h"

using namespace Pro;
using namespace Math;

Vector2::Vector2(int x, int y)
	: Vector2(static_cast<float>(x), static_cast<float>(y)) {}

Vector2::Vector2(unsigned int x, unsigned int y)
	: Vector2(static_cast<float>(x), static_cast<float>(y)) {}

Vector2::Vector2(double x, double y)
	: Vector2(static_cast<float>(x), static_cast<float>(y)) {}

Vector2::Vector2(float _x, float _y){
	x = _x;
	y = _y;
}

Vector2::Vector2(const Vector2& vec){
	x = vec.x;
	y = vec.y;
}

Vector2::Vector2(Vector2&& vec){
	x = std::move(vec.x);
	y = std::move(vec.y);
}

Vector2& Vector2::operator=(SDL_Point& p){
	this->x = p.x;
	this->y = p.y;
	return *this; 
}
Vector2& Vector2::operator=(const Vector2& p){
	this->x = p.x;
	this->y = p.y;
	return *this;
}
Vector2& Vector2::operator=(Vector2&& p){
	this->x = std::move(p.x);
	this->y = std::move(p.y);
	return *this;
} 

SDL_Point Vector2::toSDL(){
	SDL_Point o;
	o.x = static_cast<int>(x);
	o.y = static_cast<int>(y);
	return o;
}

bool Vector2::operator==(Vector2& p){
	if (x == p.x && y == p.y)
		return true;
	return false;
}

void Vector2::operator+=(Vector2& p){
	x += p.x;
	y += p.y;
}

Vector2 Pro::Math::Vector2::operator-(Vector2& other)
{
	return Vector2(x - other.x, y - other.y);
}

void Vector2::move(float _x, float _y){
	x += _x;
	y += _y;
}

bool Vector2::contains(float p){
	if ((x > p && y < p) || (x < p && y > p))
		return true;
	return false;
}

float Vector2::hypotenuse(){
	return sqrtf((x * x) + (y * y));
}

int Vector2::lContains(lua_State* L){
	Vector2* v = Util::luaP_touserdata<Vector2>(L, 1);
	lua_pushboolean(L,
		static_cast<bool>(v->contains(static_cast<float>(lua_tonumber(L, 2))))
		);
	return 1;
}
int Vector2::lHypotenuse(lua_State* L){
	Vector2* v = Util::luaP_touserdata<Vector2>(L, 1);
	lua_pushnumber(L, v->hypotenuse());
	return 1;
}

int Vector2::lMove(lua_State* L){
	Vector2* v = Util::luaP_touserdata<Vector2>(L, 1);
	v->move(static_cast<float>(lua_tonumber(L, 2)), static_cast<float>(lua_tonumber(L, 3)));
	return 0;
}

int Vector2::lGetX(lua_State* L){
	Vector2* v = Util::luaP_touserdata<Vector2>(L, 1);
	lua_pushnumber(L, v->x);
	return 1;
}
int Vector2::lGetY(lua_State* L){
	Vector2* v = Util::luaP_touserdata<Vector2>(L, 1);
	lua_pushnumber(L, v->y);
	return 1;
}
int Vector2::lSetX(lua_State* L){
	Vector2* v = Util::luaP_touserdata<Vector2>(L, 1);
	v->x = static_cast<float>(lua_tonumber(L, 2));
	return 0;
}
int Vector2::lSetY(lua_State* L){
	Vector2* v = Util::luaP_touserdata<Vector2>(L, 1);
	v->y = static_cast<float>(lua_tonumber(L, 2));
	return 0;
}
int Vector2::lGetXY(lua_State* L){
	Vector2* v = Util::luaP_touserdata<Vector2>(L, 1);
	float packed_data[] = { v->x, v->y };
	Util::luaP_pusharray<float>(L, packed_data, 2);
	return 1;
}
int Vector2::lSetXY(lua_State* L){
	Vector2* v = Util::luaP_touserdata<Vector2>(L, 1);
	v->x = luaP_tofloat(L, 2);
	v->y = luaP_tofloat(L, 3);
	return 0;
}