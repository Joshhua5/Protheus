#pragma once
#include <SDL_rect.h> 
 
namespace Pro{
	namespace Math{
		struct Vector2;
		struct Vector3;
		struct Vector4;
		// Assumes support for upto SSE3

		// Vectors

		struct Vector2 {
			float x, y;
			Vector2& operator =(SDL_Point&);
			SDL_Point toSDL_Point();
		};
		struct Vector3{ 
			float x, y, z; 
		};
		struct Vector4{
			float x, y, z, w;

			Vector4& operator =(SDL_Rect&); 
 
			SDL_Rect toSDL_Rect();
		};
	}
}