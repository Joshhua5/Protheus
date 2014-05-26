/*************************************************************************
Protheus Source File.
Copyright (C), Protheus Studios, 2013-2014.
-------------------------------------------------------------------------

Description:

-------------------------------------------------------------------------
History:
- 17:05:2014 Waring J.

*************************************************************************/
#define __SSE
#ifdef __SSE
#include <xmmintrin.h>
#endif

#pragma once
#include <SDL_rect.h>
#include "Vector2.h"
namespace Pro{
	namespace Math{

#ifdef _MSC_VER
		__declspec(align(16))
#endif
		struct Vector4{

			float x, y, z, w;
			
			Vector4(SDL_Rect& p);
			Vector4(Vector4&);
			Vector4(Vector2&, Vector2&);
			Vector4(double, double, double, double);
			Vector4(float x, float y, float z, float w);
			Vector4(int x, int y, int z, int w);
			Vector4();
			~Vector4();
			
			// z and w are width and height in that order.
			bool contains(Vector2&);
			bool overlaps(Vector4&);

			Vector4 operator+(Vector4&);
			Vector4 operator-(Vector4&);
			Vector4 operator*(Vector4&);
			Vector4 operator/(Vector4&);

			Vector4 operator=(const SDL_Rect&);
			Vector4 operator=(Vector4&);

			void operator+=(Vector4&);
			void operator-=(Vector4&);
			void operator*=(Vector4&);
			void operator/=(Vector4&);
			SDL_Rect toSDL();
		}; 
	} 
}
