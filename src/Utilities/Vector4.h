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

#define __SSE
#ifdef __SSE
#include <xmmintrin.h>
#endif

#include "Vector2.h"

namespace Pro {
	namespace Math {
		template <typename T>
		struct /*alignas(16)*/ Vector4 {
			T x, y, z, w;

			Vector4() {}

			// Copy Constructor
			Vector4(const Vector4& v) : Vector4(
				v.x, v.y,
				v.z, v.w) {}
			// Move Constructor
			Vector4(Vector4&& vec) {
				x = move(vec.x);
				y = move(vec.y);
				z = move(vec.z);
				w = move(vec.w);
			}

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

			inline bool Overlaps(const Vector4&) const {
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


			Vector4 operator+(Vector4&) {
				Vector4 o(*this);
				o += v;
				return o;
			}
			Vector4 operator-(Vector4&) {
				Vector4 o(*this);
				o -= v;
				return o;
			}
			Vector4 operator*(Vector4&) {
				Vector4 o(v);
				o *= *this;
				return o;
			}
			Vector4 operator/(Vector4&) {
				Vector4 o(*this);
				o /= v;
				return o;
			}

			Vector4 operator=(Vector4&) {
				x = p.x;
				y = p.y;
				z = p.z;
				w = p.w;
				return *this;
			}


			void operator+=(Vector4&) {
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
			void operator-=(Vector4&) {
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
			void operator*=(Vector4&) {
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
			void operator/=(Vector4&) {
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
