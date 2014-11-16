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

#define SDL_RectCreate(v) SDLP_RectCreate(v.x, v.y, v.z, v.w)
#define SDL_ColorCreate(v)  SDLP_ColorCreate((Uint8) v.x, (Uint8) v.y, (Uint8) v.z, (Uint8) v.w)

namespace Pro{
	namespace Math{  
		struct alignas(16) Vector4{
			float x, y, z, w;

			// Copy Constructor
			Vector4(const Vector4&);
			// Move Constructor
			Vector4(Vector4&&);

			Vector4(){}
			 
			Vector4(const Vector2& xy, const Vector2& zw);
			//Vector4(Vector2& xy, Vector2& zw);
			Vector4(double x, double y, double z, double w);
			Vector4(float x, float y, float z, float w);
			Vector4(int x, int y, int z, int w);

			// z and w are width and height in that order.
			bool contains(const Vector2&) const;
			bool overlaps(const Vector4&) const;

			Vector4 operator+(Vector4&);
			Vector4 operator-(Vector4&);
			Vector4 operator*(Vector4&);
			Vector4 operator/(Vector4&);
			 
			Vector4 operator=(Vector4&);

			void operator+=(Vector4&);
			void operator-=(Vector4&);
			void operator*=(Vector4&);
			void operator/=(Vector4&); 
			 
		};
	}
}
