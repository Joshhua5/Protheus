/*************************************************************************
Protheus Source File.
Copyright (C), Protheus Studios, 2013-2014.
-------------------------------------------------------------------------

Description:

-------------------------------------------------------------------------
History:
- 17:05:2014 Waring J.

*************************************************************************/
#pragma once

#include <cmath> 
#include <memory>

namespace Pro {
	namespace Math {
		template <typename T>
		struct /*alignas(16)*/ Vector2 {
			T x, y;

			Vector2() {}

			// Copy constructor
			Vector2(std::initializer_list<T> list) {
				if (list.size() < 2) {
					x = y = 0;
					return;
				}
				x = *list.begin();
				y = *(list.begin() + 1);
			}
			Vector2(const Vector2& vec) {
				x = std::move(vec.x);
				y = std::move(vec.y);
			}
			Vector2(Vector2&& vec) {
				x = std::move(vec.x);
				y = std::move(vec.y);
			}

			Vector2& operator=(const Vector2<int>& p) {
				x = static_cast<T>(p.x);
				y = static_cast<T>(p.y);
				return *this;
			}
			Vector2& operator=(const Vector2<unsigned>& p) {
				x = static_cast<T>(p.x);
				y = static_cast<T>(p.y);
				return *this;
			}

			Vector2& operator=(const Vector2<unsigned long>& p) {
				x = static_cast<T>(p.x);
				y = static_cast<T>(p.y);
				return *this;
			}

			Vector2& operator=(const Vector2<long>& p) {
				x = static_cast<T>(p.x);
				y = static_cast<T>(p.y);
				return *this;
			}

			Vector2& operator=(const Vector2<float>& p) {
				x = static_cast<T>(p.x);
				y = static_cast<T>(p.y);
				return *this;
			}

			Vector2& operator=(const Vector2<double>& p) {
				x = static_cast<T>(p.x);
				y = static_cast<T>(p.y);
				return *this;
			}

			Vector2& operator=(const Vector2<short>& p) {
				x = static_cast<T>(p.x);
				y = static_cast<T>(p.y);
				return *this;
			}

			Vector2& operator=(const Vector2<char>& p) {
				x = static_cast<T>(p.x);
				y = static_cast<T>(p.y);
				return *this;
			}

			// Move Constructor 
			Vector2& operator=(Vector2&& p) {
				x = std::move(vec.x);
				y = std::move(vec.y);
				return *this;
			}

			Vector2(T _x, T _y) {
				x = _x;
				y = _y;
			}

			bool contains(float p) const {
				return ((x > p && y < p) || (x < p && y > p));
			}

			void move(T _x, T _y) {
				x += _x;
				y += _y;
			}

			template<typename X>
			Vector2<X> cast() {
				return Vector2<X>(static_cast<X>(x), static_cast<X>(y));
			}

			float length()const {
				return sqrtf((x * x) + (y * y));
			}

			Vector2 normalize() const {
				Vector2 out(*this);
				out /= out.length();
				return out;
			}

			Vector2& operator=(const T& p) {
				x = p;
				y = p;
				return *this;
			}

			//Vector2& operator=(const Vector2<unsigned>& rhs){
			//	x = static_cast<T>(rhs.x);
			//	y = static_cast<T>(rhs.y);
			//	return *this;

			//}

			bool operator==(const Vector2& p) {
				return x == p.x && y == p.y;
			}

			bool operator!=(const Vector2& p) {
				return x != p.x || y != p.y;
			}

			void operator+=(T val) {
				x += val;
				y += val;
			}
			void operator+=(Vector2& p) {
				x += p.x;
				y += p.y;
			}
			Vector2 operator+(Vector2& val) {
				Vector2 out(*this);
				out += val;
				return out;
			}
			Vector2 operator+(T val) {
				Vector2 out(*this);
				out += val;
				return out;
			}

			Vector2 operator-(T val) {
				Vector2 out(*this);
				out -= val;
				return out;
			}
			Vector2 operator-(Vector2& val) {
				Vector2 out(*this);
				out -= val;
				return out;
			}
			void operator-=(T val) {
				x -= val;
				y -= val;
			}
			void operator-=(Vector2& p) {
				x -= p.x;
				y -= p.y;
			}

			Vector2 operator/(T val) {
				Vector2 out(*this);
				out /= val;
				return out;
			}
			Vector2 operator/(Vector2& val) {
				Vector2 out(*this);
				out /= val;
				return out;
			}
			void operator/=(T val) {
				x /= val;
				y /= val;
			}
			void operator/=(Vector2& p) {
				x /= p.x;
				y /= p.y;
			}

			Vector2 operator*(T val) {
				Vector2 out(*this);
				out *= val;
				return out;
			}
			Vector2 operator*(Vector2& val) {
				Vector2 out(*this);
				out *= val;
				return out;
			}
			void operator*=(T val) {
				x *= val;
				y *= val;
			}
			void operator*=(Vector2& p) {
				x *= p.x;
				y *= p.y;
			}
		};

	}
}