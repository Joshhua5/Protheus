/*************************************************************************
Protheus Source File.
Copyright (C), Protheus Studios, 2013-2014.
-------------------------------------------------------------------------

Description:

-------------------------------------------------------------------------
History:
- 21:12:2014 Waring J.

*************************************************************************/
 
#include "Vector2.h"
#include "Vector3.h"
#include "Vector4.h"
#include "Matrix33.h"
#include "Matrix44.h"

#pragma once
namespace Pro {
	namespace Math {
		/*! PI constant*/
		const float pi = 3.1415926535897932384626433832795028841971f;

		/*!
			Convert radians to degrees
		*/
		inline float radToDeg(const float rad) {
			return rad * 180 / pi; 
		}

		/*!
		Convert degrees to radians
		*/
		inline float degToRad(const float deg) {
			return deg * pi / 180;
		}

		template<typename T>
		inline Vector2<T> toVector2(Vector3<T>&& rhs) {
			return Vector2<T>(rhs.x, rhs.y);
		}

		template<typename T>
		inline Vector2<T> toVector2(Vector4<T>&& rhs) {
			return Vector2<T>(rhs.x, rhs.y);
		}

		template<typename T>
		inline Vector2<T> toVector2(const Vector3<T>& rhs) {
			return Vector2<T>(rhs.x, rhs.y);
		}

		template<typename T>
		inline Vector2<T> toVector2(const Vector4<T>& rhs) {
			return Vector2<T>(rhs.x, rhs.y);
		} 
		 
		template<typename T>
		inline Vector3<T> toVector3(Vector4<T>&& rhs) {
			return Vector3<T>(rhs.x, rhs.y, rhs.z);
		}

		template<typename T>
		inline Vector3<T> toVector3(Vector2<T>&& rhs) {
			return Vector3<T>(rhs.x, rhs.y, T());
		}

		template<typename T>
		inline Vector3<T> toVector3(const Vector4<T>& rhs) {
			return Vector3<T>(rhs.x, rhs.y, rhs.z);
		}

		template<typename T>
		inline Vector3<T> toVector3(const Vector2<T>& rhs) {
			return Vector3<T>(rhs.x, rhs.y, T());
		}

		template<typename T>
		inline Vector4<T> toVector4(Vector3<T>&& rhs) {
			return Vector4<T>(rhs.x, rhs.y, rhs.z, T());
		}

		template<typename T>
		inline Vector4<T> toVector4(Vector2<T>&& rhs) {
			return Vector4<T>(rhs.x, rhs.y, T(), T());
		}
	
		template<typename T>
		inline Vector4<T> toVector4(const Vector3<T>& rhs) {
			return Vector4<T>(rhs.x, rhs.y, rhs.z, T());
		}

		template<typename T>
		inline Vector4<T> toVector4(const Vector2<T>& rhs) {
			return Vector4<T>(rhs.x, rhs.y, T(), T());
		}
		 
		template<typename T>
		inline T prand(T min, T max){
			static bool executed = false;
			if (executed++)
				srand(time(NULL));
			return static_cast<T>((rand() % max - min) + min);
		} 
	}
}