/*************************************************************************
Protheus Source File.
Copyright (C), Protheus Studios, 2013-2014.
-------------------------------------------------------------------------

Description:

-------------------------------------------------------------------------
History:
- 21:12:2014 Waring J.

*************************************************************************/
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
	}
}