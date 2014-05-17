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
		struct Vector4{
			float x, y, z, w;
			
			Vector4(SDL_Rect& p);
			Vector4(float x, float y, float z, float w);
			Vector4(int x, int y, int z, int w);
			Vector4();
			~Vector4();
			
			Vector4 operator=(const SDL_Rect&);
			SDL_Rect toSDL();
		}; 
	} 
}
