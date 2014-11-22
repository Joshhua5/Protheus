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
	x = std::move(vec.x);
	y = std::move(vec.y);
}
Vector2::Vector2(Vector2&& vec){
	x = std::move(vec.x);
	y = std::move(vec.y);
}

//inline Vector2& Vector2::operator=(const Vector2& p){
//	x = p.x;
//	y = p.y;
//	return *this;
//}

//inline Vector2& Vector2::operator=(Vector2&& p);

inline Vector2& Vector2::operator=(float p){
	x = p;
	y = p;
	return *this;
}

//inline bool Vector2::operator==(const Vector2& p)

inline void Vector2::operator+=(Vector2& p){
	x += p.x;
	y += p.y;
}

inline void Vector2::operator+=(float val){
	x += val;
	y += val;
}

inline Vector2 Vector2::operator+(Vector2& val){
	Vector2 out(*this);
	out += val;
	return out;
}

inline Vector2 Vector2::operator+(float val){
	Vector2 out(*this);
	out += val;
	return out;
}

inline void Vector2::operator-=(Vector2& p){
	x -= p.x;
	y -= p.y;
}

inline void Vector2::operator-=(register float val){
	x -= val;
	y -= val;
}

inline Vector2 Vector2::operator-(Vector2& val){
	Vector2 out(*this);
	out -= val;
	return out;
}

inline Vector2 Vector2::operator-(float val){
	Vector2 out(*this);
	out -= val;
	return out;
}

inline void Vector2::operator/=(Vector2& p){
	x /= p.x;
	y /= p.y;
}

inline void Vector2::operator/=(float val){
	x /= val;
	y /= val;
}

inline Vector2 Vector2::operator/(Vector2& val){
	Vector2 out(*this);
	out /= val;
	return out;
}

inline Vector2 Vector2::operator/(float val){
	Vector2 out(*this);
	out /= val;
	return out;
}

inline void Vector2::operator*=(Vector2& p){
	x *= p.x;
	y *= p.y;
}

inline void Vector2::operator*=(float val){
	x *= val;
	y *= val;
}

inline Vector2 Vector2::operator*(Vector2& val){
	Vector2 out(*this);
	out *= val;
	return out;
}

inline Vector2 Vector2::operator*(float val){
	Vector2 out(*this);
	out *= val;
	return out;
}

inline void Vector2::move(float _x, float _y){
	x += _x;
	y += _y;
}

inline bool Vector2::contains(float p) const{
	if ((x > p && y < p) || (x < p && y > p))
		return true;
	return false;
}

inline float Vector2::length() const{
	return sqrtf((x * x) + (y * y));
}

inline Vector2 Vector2::normalize() const{
	Vector2 out(*this);
	out /= out.length();
	return out;
}
 