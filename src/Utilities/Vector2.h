/*************************************************************************
Protheus Source File.
Copyright (C), Protheus Studios, 2013-2015.
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
			inline Vector2(std::initializer_list<T> list) {
				if (list.size() < 2) {
					x = y = 0;
					return;
				}
				x = *list.begin();
				y = *(list.begin() + 1);
			}
			inline Vector2(const Vector2& vec) {
				x = std::move(vec.x);
				y = std::move(vec.y);
			}
			inline Vector2(Vector2&& vec) {
				x = std::move(vec.x);
				y = std::move(vec.y);
			}

			inline Vector2& operator=(const Vector2<int>& p) {
				x = static_cast<T>(p.x);
				y = static_cast<T>(p.y);
				return *this;
			}
			inline Vector2& operator=(const Vector2<unsigned>& p) {
				x = static_cast<T>(p.x);
				y = static_cast<T>(p.y);
				return *this;
			}

			inline Vector2& operator=(const Vector2<unsigned long>& p) {
				x = static_cast<T>(p.x);
				y = static_cast<T>(p.y);
				return *this;
			}

			inline Vector2& operator=(const Vector2<long>& p) {
				x = static_cast<T>(p.x);
				y = static_cast<T>(p.y);
				return *this;
			}

			inline Vector2& operator=(const Vector2<float>& p) {
				x = static_cast<T>(p.x);
				y = static_cast<T>(p.y);
				return *this;
			}

			inline Vector2& operator=(const Vector2<double>& p) {
				x = static_cast<T>(p.x);
				y = static_cast<T>(p.y);
				return *this;
			}

			inline Vector2& operator=(const Vector2<short>& p) {
				x = static_cast<T>(p.x);
				y = static_cast<T>(p.y);
				return *this;
			}

			inline Vector2& operator=(const Vector2<char>& p) {
				x = static_cast<T>(p.x);
				y = static_cast<T>(p.y);
				return *this;
			}

			// Move Constructor 
			Vector2& operator=(Vector2&& p) {
				x = std::move(p.x);
				y = std::move(p.y);
				return *this;
			}

			inline Vector2(T _x, T _y) {
				x = _x;
				y = _y;
			}

			//! Checks if p is between the x and y value
			inline bool Contains(float p) const {
				return ((x > p && y < p) || (x < p && y > p));
			}

			inline void Move(T _x, T _y) {
				x += _x;
				y += _y;
			}

			inline bool Equals(T _x, T _y, T _z) {
				return x == _x && y == _y && z == _z;
			}

			template<typename X>
			Vector2<X> Cast() const {
				return Vector2<X>(
					static_cast<X>(x),
					static_cast<X>(y));
			} 

			inline T Length() const {
					return sqrtf((x * x) + (y * y));
			}

			inline Vector2 Normalize() const {
				Vector2 out(*this);
				out /= out.Length();
				return out;
			}

			inline Vector2& operator=(const T& p) {
				x = p;
				y = p;
				return *this;
			}
			 
			inline bool operator==(const Vector2& p) {
				return x == p.x && y == p.y;
			}

			inline bool operator!=(const Vector2& p) {
				return x != p.x || y != p.y;
			}

			inline void operator+=(T val) {
				x += val;
				y += val;
			}
			inline void operator+=(Vector2& p) {
				x += p.x;
				y += p.y;
			}
			inline Vector2 operator+(Vector2& val) {
				Vector2 out(*this);
				out += val;
				return out;
			}
			inline Vector2 operator+(T val) {
				Vector2 out(*this);
				out += val;
				return out;
			}

			inline Vector2 operator-(T val) {
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

			/*! Returns a pointer to the internal structure's data
			*/
			const T* data() const {
				return &x;
			}
			T* data() {
				return &x;
			}
		};

	}
}