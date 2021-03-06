/*************************************************************************
Protheus Source File.
Copyright (C), Protheus Studios, 2013-2016.
-------------------------------------------------------------------------

Description:

-------------------------------------------------------------------------
History:
- 21:12:2014 Waring J.

*************************************************************************/
#pragma once

#include "Vector2.h"
#include "Vector3.h"
#include "Vector4.h"
#include "Matrix33.h"
#include "Matrix44.h"
#include "SIMD.h"

namespace Pro {
	namespace Math {
		/*! PI constant*/
		const float PI = 3.1415926535897932384626433832795028841971f;
		const float RadiansToDegrees = 180.0f / PI;
		const float DegreesToRadians = PI / 180.0f;

		/*!
			Convert radians to degrees
			*/
		inline float RadToDeg(const float rad) {
			return rad * RadiansToDegrees;
		}

		/*!
		Convert degrees to radians
		*/
		inline float DegToRad(const float deg) {
			return deg * DegreesToRadians;
		}

		template<typename T>
		inline Vector2<T> ToVector2(Vector3<T>&& rhs) {
			return Vector2<T>(rhs.x, rhs.y);
		}

		template<typename T>
		inline Vector2<T> ToVector2(Vector4<T>&& rhs) {
			return Vector2<T>(rhs.x, rhs.y);
		}

		template<typename T>
		inline Vector2<T> ToVector2(const Vector3<T>& rhs) {
			return Vector2<T>(rhs.x, rhs.y);
		}

		template<typename T>
		inline Vector2<T> ToVector2(const Vector4<T>& rhs) {
			return Vector2<T>(rhs.x, rhs.y);
		}

		template<typename T>
		inline Vector3<T> ToVector3(Vector4<T>&& rhs)  {
			return Vector3<T>(rhs.x, rhs.y, rhs.z);
		}

		template<typename T>
		inline Vector3<T> ToVector3(Vector2<T>&& rhs) {
			return Vector3<T>(rhs.x, rhs.y, T());
		}

		template<typename T>
		inline Vector3<T> ToVector3(Vector2<T>&& xy, T&& z) {
			return Vector3<T>(xy.x, xy.y, z);
		}

		template<typename T>
		inline Vector3<T> ToVector3(const Vector4<T>& rhs) {
			return Vector3<T>(rhs.x, rhs.y, rhs.z);
		}

		template<typename T>
		inline Vector3<T> ToVector3(const Vector2<T>& rhs) {
			return Vector3<T>(rhs.x, rhs.y, T());
		}

		template<typename T>
		inline Vector3<T> ToVector3(const Vector2<T>&& xy, const T& z) {
			return Vector3<T>(xy.x, xy.y, z);
		}

		template<typename T>
		inline Vector4<T> ToVector4(Vector3<T>&& rhs) {
			return Vector4<T>(rhs.x, rhs.y, rhs.z, T());
		}

		template<typename T>
		inline Vector4<T> ToVector4(Vector2<T>&& rhs) {
			return Vector4<T>(rhs.x, rhs.y, T(), T());
		}

		template<typename T>
		inline Vector4<T> ToVector4(const Vector3<T>& rhs) {
			return Vector4<T>(rhs.x, rhs.y, rhs.z, T());
		}

		template<typename T>
		inline Vector4<T> ToVector4(const Vector2<T>& rhs) {
			return Vector4<T>(rhs.x, rhs.y, T(), T());
		}

		template<typename T>
		inline Vector4<T> ToVector4(Vector3<T>&& rhs, T&& w) {
			return Vector4<T>(rhs.x, rhs.y, rhs.z, w);
		}

		template<typename T>
		inline Vector4<T> ToVector4(Vector2<T>&& rhs, T&& z, T&& w) {
			return Vector4<T>(rhs.x, rhs.y, z, w);
		}

		template<typename T>
		inline Vector4<T> ToVector4(const Vector3<T>& rhs, const T& w) {
			return Vector4<T>(rhs.x, rhs.y, rhs.z, w);
		}

		template<typename T>
		inline Vector4<T> ToVector4(const Vector2<T>& rhs, const T& z, const T& w) {
			return Vector4<T>(rhs.x, rhs.y, z, w);
		}

		//! Min and Max to a maximum of 2 decimal points
		template<typename T>
		inline T Rand(T min, T max){
			static bool executed = false;
			if (executed) {
				srand((int)time(NULL));
				executed = true;
			}

			min *= 100;
			max *= 100;

			return static_cast<T>(((rand() % static_cast<long long>(max - min)) + min) / 100);
		}
	}
}