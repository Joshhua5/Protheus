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

#include "PreDefinition.h"
#if defined(SSE) 
#include <xmmintrin.h>
#elif defined(NANO)

#endif

#include "Matrix44.h"
#include "Vector2.h"

namespace Pro {
	namespace Math {
		template <typename T>
		struct alignas(16) Vector4 {
            union{
                struct{ T x, y, z, w; };
                T m_[4];
            };
			Vector4() {}

			Vector4(T value) {
				x = y = z = w = value;
			}
			// Copy Constructor
			Vector4(const Vector4& v) : Vector4(
				v.x, v.y,
				v.z, v.w) {}

			Vector4(const Vector2<T>& pos, const Vector2<T>& dim) : Vector4(
				pos.x, pos.y,
				dim.x, dim.y) {}

			Vector4(T _x, T _y, T _z, T _w) {
				x = _x;
				y = _y;
				z = _z;
				w = _w;
			}

			// z and w are width and height in that order.
			inline bool Contains(const Vector2<T>& v) const {
				return
					v.x > x &&
					x + z > v.x &&
					v.y > y &&
					x + y > v.y;
			}

			inline bool Overlaps(const Vector4& v) const {
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

			inline void Move(T _x, T _y, T _z, T _w) {
				x += _x;
				y += _y;
				z += _z;
				w += _w;
			}

			inline void Set(T _x, T _y, T _z, T _w) {
				this->x = _x;
				this->y = _y;
				this->z = _z;
				this->w = _w;
			}

			inline bool Equals(T _x, T _y, T _z, T _w) {
				return x == _x && y == _y && z == _z && w == _w;
			}

			template<typename X>
			Vector4<X> Cast() const {
				return Vector4<X>(
					static_cast<X>(x),
					static_cast<X>(y),
					static_cast<X>(z),
					static_cast<X>(w));
			}

			inline T Length() const {
				return sqrtf((x * x) + (y * y) + (z * z) + (w * w));
			}

			inline Vector4 Normalize() const {
				Vector4 out(*this);
				out /= out.Length();
				return out;
			}

			inline Vector4& NormalizeThis() {
				*this /= Length();
				return *this;
			}

			Vector4 operator+(const Vector4& v) const {
				Vector4 o(*this);
				o += v;
				return o;
			}
			Vector4 operator-(const Vector4& v) const {
				Vector4 o(*this);
				o -= v;
				return o;
			}
            
            Vector4 operator*(const Matrix44& m) const{
                Vector4 o;
                for(int x = 0; x < 4; ++x){
                    o.m_[x] = m.matrix_[0][x] * m_[0] +
                              m.matrix_[1][x] * m_[1] +
                              m.matrix_[2][x] * m_[2] +
                              m.matrix_[3][x] * m_[3];
                }
                return o;
            }
            
			Vector4 operator*(const Vector4& v) const {
				Vector4 o(v);
				o *= *this;
				return o;
			}
			Vector4 operator/(const Vector4& v) const {
				Vector4 o(*this);
				o /= v;
				return o;
			}
			 

			bool operator==(const Vector4& rhs) { 
				return x == rhs.x && y == rhs.y && z == rhs.z && w == rhs.w;
			}

			bool operator!=(const Vector4& rhs) { 
				return !(x == rhs.x && y == rhs.y && z == rhs.z  && w == rhs.w);
			}

			Vector4 operator=(const Vector4& v) {
				x = v.x;
				y = v.y;
				z = v.z;
				w = v.w;
				return *this;
			}
/*
            float operator[](unsigned i){
                return *((float*)this + i);
                
                switch(i){
                    case 0:
                        return x;
                    case 1:
                        return y;
                    case 2:
                        return z;
                    case 3:
                        return w;
                    default:
                        return 0;
                }
            }*/

			void operator+=(const Vector4& v) {
#ifdef SEE
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
			void operator-=(const Vector4& v) {
#ifdef SEE
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
			void operator*=(const Vector4& v) {
#ifdef SEE
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
        
			void operator/=(const Vector4& v) {
#ifdef SEE
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
		};
	}
}
