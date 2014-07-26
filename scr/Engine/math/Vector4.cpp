#include "Vector4.h"

using namespace Pro;
using namespace Math;

Vector4::Vector4(const Vector4& v) : Vector4(
	v.x, v.y,
	v.z, v.w){}

Vector4::Vector4(const Vector2& pos,const Vector2& dim) : Vector4(
	pos.x, pos.y,
	dim.x, dim.y){}

Vector4::Vector4(double x, double y, double z, double w) : Vector4(
	static_cast<float>(x),
	static_cast<float>(y),
	static_cast<float>(z),
	static_cast<float>(w))
{}
 
Vector4::Vector4(int x, int y, int z, int w) : Vector4(
	static_cast<float>(x),
	static_cast<float>(y),
	static_cast<float>(z),
	static_cast<float>(w))
{}

Vector4::Vector4(float _x, float _y, float _z, float _w){
	x = _x;
	y = _y;
	z = _z;
	w = _w;
}

Vector4::Vector4(Vector4&& vec){
	x = move(vec.x);
	y = move(vec.y);
	z = move(vec.z);
	w = move(vec.w);
}

bool Vector4::contains(const Math::Vector2& v) const{
	if (v.x > x &&
		x + z > v.x &&
		v.y > y &&
		x + y > v.y)
		return true;
	return false;
}

bool Vector4::overlaps(const Vector4& v) const{
	if ((x >= v.x && x + w <= v.x + v.x) &
		((y + w <= v.y && y >= v.y) |
		(y + w > v.y && y + w < v.y + v.y)))
		return true;
	// check left and right
	else if (y > v.y &&  y + w < v.y + v.y &&
		((x + z > v.x &&  x + z < v.x + v.x) ||
		(x > v.x + v.x && x < v.x + v.x)))
		return true;
	return false;
}
Vector4 Vector4::operator+(Vector4& v){
	Vector4 o(*this);
	o += v;
	return o;
}
Vector4 Vector4::operator-(Vector4& v){
	Vector4 o(*this);
	o -= v;
	return o;
}
Vector4 Vector4::operator*(Vector4& v){
	Vector4 o(v);
	o *= *this;
	return o;
}
Vector4 Vector4::operator/(Vector4& v){
	Vector4 o(*this);
	o /= v;
	return o;
}
 
Vector4 Vector4::operator=(Vector4& p){ return Vector4(p.x, p.y, p.z, p.w); }

void Vector4::operator+=(Vector4& v){
#ifdef __SSE
	// SSE code
	__m128 m1 = _mm_loadu_ps(&x);
	__m128 m2 = _mm_loadu_ps(&v.x);
	_mm_storeu_ps(&x, _mm_add_ps(m1, m2));

#else
	// Scalar code

	x += v.x;
	y += v.y;
	z += v.z;
	w += v.w;
#endif
}
void Vector4::operator-=(Vector4& v){
#ifdef __SSE
	// SSE code
	__m128 m1 = _mm_loadu_ps(&x);
	__m128 m2 = _mm_loadu_ps(&v.x);
	_mm_storeu_ps(&x, _mm_sub_ps(m1, m2));

#else
	// Scalar code

	x -= v.x;
	y -= v.y;
	z -= v.z;
	w -= v.w;
#endif
}
void Vector4::operator*=(Vector4& v){
#ifdef __SSE
	// SSE code
	__m128 m1 = _mm_loadu_ps(&x);
	__m128 m2 = _mm_loadu_ps(&v.x);
	_mm_storeu_ps(&x, _mm_mul_ps(m1, m2));

#else
	// Scalar code

	x *= v.x;
	y *= v.y;
	z *= v.z;
	w *= v.w;
#endif
}
void Vector4::operator/=(Vector4& v){
#ifdef __SSE
	// SSE code
	__m128 m1 = _mm_loadu_ps(&x);
	__m128 m2 = _mm_loadu_ps(&v.x);
	_mm_storeu_ps(&x, _mm_div_ps(m1, m2));

#else
	// Scalar code

	x /= v.x;
	y /= v.y;
	z /= v.z;
	w /= v.w;
#endif
}
 
int Vector4::lGetX(lua_State* L){
	Vector4* v = Util::luaP_touserdata<Vector4>(L, 1);
	lua_pushnumber(L, v->x);
	return 1;
}
int Vector4::lGetY(lua_State* L){
	Vector4* v = Util::luaP_touserdata<Vector4>(L, 1);
	lua_pushnumber(L, v->y);
	return 1;
}
int Vector4::lGetZ(lua_State* L){
	Vector4* v = Util::luaP_touserdata<Vector4>(L, 1);
	lua_pushnumber(L, v->z);
	return 1;
}
int Vector4::lGetW(lua_State* L){
	Vector4* v = Util::luaP_touserdata<Vector4>(L, 1);
	lua_pushnumber(L, v->w);
	return 1;
}
int Vector4::lSetX(lua_State* L){
	Vector4* v = Util::luaP_touserdata<Vector4>(L, 1);
	v->x = static_cast<float>(lua_tonumber(L, 2));
	return 0;
}
int Vector4::lSetY(lua_State* L){
	Vector4* v = Util::luaP_touserdata<Vector4>(L, 1);
	v->x = static_cast<float>(lua_tonumber(L, 2));
	return 0;
}
int Vector4::lSetZ(lua_State* L){
	Vector4* v = Util::luaP_touserdata<Vector4>(L, 1);
	v->x = static_cast<float>(lua_tonumber(L, 2));
	return 0;
}
int Vector4::lSetW(lua_State* L){
	Vector4* v = Util::luaP_touserdata<Vector4>(L, 1);
	v->x = static_cast<float>(lua_tonumber(L, 2));
	return 0;
}
int Vector4::lGetXYZW(lua_State* L){
	Vector4* v = Util::luaP_touserdata<Vector4>(L, 1);
	lua_pushnumber(L, v->w);
	return 1;
}
int Vector4::lSetXYZW(lua_State* L){
	Vector4* v = Util::luaP_touserdata<Vector4>(L, 1);
	v->x = static_cast<float>(lua_tonumber(L, 2));
	return 0;
}