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
#include <SDL_rect.h>

namespace Pro{
	namespace Math{
		struct Vector2 {
			float x, y;

			Vector2(SDL_Point);
			Vector2(unsigned int, unsigned int);
			Vector2(float, float);
			Vector2(int, int);
			Vector2();
			~Vector2();

			bool contains(float&);

			Vector2 operator=(SDL_Point&);
			SDL_Point operator=(Vector2&);
			bool operator==(Vector2&);
			void operator+=(Vector2&); 

		}; 
	}
}


