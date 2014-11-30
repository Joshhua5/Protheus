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

			// Copy constructor
			Vector2(const Vector2& vec) {
				x = std::move(vec.x);
				y = std::move(vec.y);
			}

			Vector2& operator=(const Vector2& p) {
				x = p.x;
				y = p.y;
				return *this;
			}
			// Move Constructor

			Vector2(Vector2&& vec) {
				x = std::move(vec.x);
				y = std::move(vec.y);
			}

			Vector2& operator=(Vector2&& p) {
				x = std::move(vec.x);
				y = std::move(vec.y);
				return *this;
			}

			Vector2() {}

			Vector2(T _x, T _y) {
				x = _x;
				y = _y;
			}

			bool contains(float p) const {
				return ((x > p && y < p) || (x < p && y > p))
					? true : false;
			}

			void move(T _x, T _y) {
				x += _x;
				y += _y;
			}

			float length()const {
				return sqrtf((x * x) + (y * y));
			}

			Vector2 normalize() const {
				Vector2 out(*this);
				out /= out.length();
				return out;
			}

			Vector2& operator=(T p) {
				x = p;
				y = p;
				return *this;
			}

			bool operator==(const Vector2& p) {
				return (x == p.x && y == p.y) ? true : false;
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