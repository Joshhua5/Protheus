/*************************************************************************
Protheus Source File.
Copyright (C), Protheus Studios, 2013-2014.
-------------------------------------------------------------------------

Description:

-------------------------------------------------------------------------
History:
- 17:05:2014 Waring J.

*************************************************************************/

#include "Vector4.h"

using namespace Pro;
using namespace Math;
Vector4::Vector4(const Vector4& v) : Vector4(
	v.x, v.y,
	v.z, v.w){}

Vector4::Vector4(Vector2& pos, Vector2& dim) : Vector4(
	pos.x, pos.y,
	dim.x, dim.y){}

Vector4::Vector4(double x, double y, double z, double w) : Vector4(
	static_cast<float>(x),
	static_cast<float>(y),
	static_cast<float>(z),
	static_cast<float>(w))
{}

Vector4::Vector4(SDL_Rect& p) : Vector4(
	static_cast<float>(p.x),
	static_cast<float>(p.y),
	static_cast<float>(p.w),
	static_cast<float>(p.h))
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

Vector4::Vector4()
{
	x = y = z = w = 0;
}


Vector4::~Vector4()
{
}

bool Vector4::contains(Math::Vector2& v){
	if (v.x > x &&
		x + z > v.x &&
		v.y > y &&
		x + y > v.y)
		return true;
	return false;
}

bool Vector4::overlaps(Vector4& v){
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
	Vector4 o(*this);
	o *= v;
	return o;
}
Vector4 Vector4::operator/(Vector4& v){
	Vector4 o(*this);
	o /= v;
	return o;
}

Vector4 Vector4::operator=(const SDL_Rect& p){ return Vector4(p.x, p.y, p.w, p.h); }
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

SDL_Rect Vector4::toSDL(){
	SDL_Rect o;
	o.x = static_cast<int>(x);
	o.y = static_cast<int>(y);
	o.w = static_cast<int>(z);
	o.h = static_cast<int>(w);
	return o;
}