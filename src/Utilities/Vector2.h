/*************************************************************************
Protheus Source File.
Copyright (C), Protheus Studios, 2013-2016.
-------------------------------------------------------------------------

Description:

-------------------------------------------------------------------------
History:
- 17:05:2014 Waring J.

*************************************************************************/
#pragma once

#include <math.h> 
#include <memory>

namespace Pro {
	namespace Math {
		template <typename T>
		struct alignas(16) Vector2 {
            union{
                struct{ T x, y; };
                float m_[2];
            };

			Vector2() {}

			Vector2(T value) {
				x = y = value;
			}

			Vector2(T x_, T y_) {
				x = x_;
				y = y_;
			}

			// Copy constructor
			Vector2(const Vector2& vec) {
				x = std::move(vec.x);
				y = std::move(vec.y);
			}  

			//! Checks if p is between the x and y value
			inline bool Contains(T p) const {
				return ((x > p && y < p) || (x < p && y > p));
			}

			inline void Move(T _x, T _y) {
				x += _x;
				y += _y;
			}

			inline bool Equals(T _x, T _y) {
				return x == _x && y == _y;
			}

			inline void Set(T _x, T _y) {
				this->x = _x;
				this->y = _y;
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
			
			inline Vector2 NormalizeThis() {  
				*this /= Length(); 
				return *this;
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

			inline void operator+=(const T val) {
				x += val;
				y += val;
			}
			inline void operator+=(const Vector2& p) {
				x += p.x;
				y += p.y;
			}
			inline Vector2 operator+(const Vector2& val) const {
				Vector2 out(*this);
				out += val;
				return out;
			}
			inline Vector2 operator+(const T val) const {
				Vector2 out(*this);
				out += val;
				return out;
			}

			inline Vector2 operator-(const T val) const {
				Vector2 out(*this);
				out -= val;
				return out;
			}
			Vector2 operator-(const Vector2& val) const {
				Vector2 out(*this);
				out -= val;
				return out;
			}
			void operator-=(T val) {
				x -= val;
				y -= val;
			}
			void operator-=(const Vector2& p) {
				x -= p.x;
				y -= p.y;
			}

			Vector2 operator/(T val) const {
				Vector2 out(*this);
				out /= val;
				return out;
			}
			Vector2 operator/(const Vector2& val) const {
				Vector2 out(*this);
				out /= val;
				return out;
			}
			void operator/=(T val) {
				x /= val;
				y /= val; 
			}
			void operator/=(const Vector2& p) {
				x /= p.x;
				y /= p.y;
			}

			Vector2 operator*(T val) const {
				Vector2 out(*this);
				out *= val;
				return out;
			}
			Vector2 operator*(const Vector2& val) const {
				Vector2 out(*this);
				out *= val;
				return out;
			}
			void operator*=(T val) {
				x *= val;
				y *= val;
			}
			void operator*=(const Vector2& p) {
				x *= p.x;
				y *= p.y;
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
			  
		};

	}
}