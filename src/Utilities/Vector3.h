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
#include "Matrix33.h"
 
namespace Pro {
	namespace Math {
		template<typename T = float>
		struct Vector3 {
            union{
                struct{T x, y, z;};
                float m_[3];
            };
			 
			Vector3() {}

			Vector3(T value) {
				x = y = z = value;
			}

			Vector3(T _x, T _y, T _z) {
				x = _x;
				y = _y;
				z = _z;
			} 
			 
			Vector3(const Vector3& rhs) {
				x = rhs.x;
				y = rhs.y;
				z = rhs.z;
			}
            
            Vector3& operator=(const Vector3& rhs) {
                x = rhs.x;
                y = rhs.y;
                z = rhs.z;
                return *this;
            }

			bool operator!=(const Vector3& rhs) const {
				return !(x == rhs.x && y == rhs.y && z == rhs.z);
			}

			bool operator==(const Vector3& rhs) const {
				return x == rhs.x && y == rhs.y && z == rhs.z;
			}

			inline void Move(T _x, T _y, T _z) {
				x += _x;
				y += _y;
				z += _z;
			}

			inline bool Equals(T _x, T _y, T _z) const {
				return x == _x && y == _y && z == _z;
			}

			inline void Set(T _x, T _y, T _z) {
				this->x = _x;
				this->y = _y;
				this->z = _z;
			}
			 
			template<typename X>
			Vector3<X> Cast() const {
				return Vector3<X>(
					static_cast<X>(x),
					static_cast<X>(y),
					static_cast<X>(z));
			}

			inline T Length() const {
				return sqrtf((x * x) + (y * y) + (z * z));
			} 

			inline Vector3 Normalize() const {
				Vector3 out(*this);
				out /= Length();
				return out;
			}

			inline Vector3& NormalizeThis() { 
				*this /= Length();
				return *this;
			}

			Vector3 operator+(const Vector3& rhs) const {
				Vector3<T> out;
				out.x = x + rhs.x;
				out.y = y + rhs.y;
				out.z = z + rhs.z;
				return out;
			}
			Vector3 operator-(const Vector3& rhs) const {
				Vector3<T> out;
				out.x = x - rhs.x;
				out.y = y - rhs.y;
				out.z = z - rhs.z;
				return out;
			}
			Vector3 operator*(const Vector3& rhs) const {
				Vector3<T> out;
				out.x = x * rhs.x;
				out.y = y * rhs.y;
				out.z = z * rhs.z;
				return out;
			}
			Vector3 operator/(const Vector3& rhs) const {
				Vector3<T> out;
				out.x = x / rhs.x;
				out.y = y / rhs.y;
				out.z = z / rhs.z;
				return out;
			} 

			Vector3 operator*(const Matrix33<T>& m) const {
				Vector3 o;
				for (int x = 0; x < 3; ++x) {
					o.m_[x] =
						m.matrix_[0][x] * m_[0] +
						m.matrix_[1][x] * m_[1] +
						m.matrix_[2][x] * m_[2];
				}
				return o;
			}
			  
			Vector3 operator-() const {
				return{ -x, -y, -z };
			}

			Vector3 operator+() const {
				return{ +x, +y, +z };
			}

			void operator+=(const Vector3& rhs) {
				x += rhs.x;
				y += rhs.y;
				z += rhs.z;
			}
			void operator-=(const Vector3& rhs) {
				x -= rhs.x;
				y -= rhs.y;
				z -= rhs.z;
			}
			void operator/=(const Vector3& rhs) {
				x /= rhs.x;
				y /= rhs.y;
				z /= rhs.z;
			}
			void operator*=(const Vector3& rhs) {
				x *= rhs.x;
				y *= rhs.y;
				z *= rhs.z;
			}
		};
	}
}