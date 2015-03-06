/*************************************************************************
Protheus Source File.
Copyright (C), Protheus Studios, 2013-2015.
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
		const float PI = 3.1415926535897932384626433832795028841971f;

		/*!
			Convert radians to degrees
		*/
		inline float RadToDeg(const float rad) {
			return rad * 180 / PI; 
		}

		/*!
		Convert degrees to radians
		*/
		inline float DegToRad(const float deg) {
			return deg * PI / 180;
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
		inline Vector3<T> ToVector3(Vector4<T>&& rhs) {
			return Vector3<T>(rhs.x, rhs.y, rhs.z);
		}

		template<typename T>
		inline Vector3<T> ToVector3(Vector2<T>&& rhs) {
			return Vector3<T>(rhs.x, rhs.y, T());
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
		 
		//! Min and Max to a maximum of 2 decimal points
		template<typename T>
		inline T Rand(T min, T max){
			static bool executed = false;
			if (executed) {
				srand(time(NULL));
				executed = true;
			}

			min *= 100;
			max *= 100;
			 
			return static_cast<T>(((rand() % static_cast<long long>(max - min)) + min) / 100);
		} 
	}
}