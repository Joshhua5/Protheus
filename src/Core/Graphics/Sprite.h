/*************************************************************************
Protheus Source File.
Copyright (C), Protheus Studios, 2013-2014.
-------------------------------------------------------------------------

Description:

-------------------------------------------------------------------------
History:
- 20:05:2014: Waring J.
*************************************************************************/

#pragma once

#include <SDL.h> 
#include <Vector2.h>
#include <string>

namespace Pro{
	namespace Asset{
		using namespace std;
		using namespace Math; 

		class Sprite 
		{
		private: 
			SDL_Texture* texture; 
			SDL_Point center;

			Vector2 dimensions;

		public: 
			Sprite(SDL_Texture* tex);
			~Sprite();

			SDL_Texture* getTexture();
			SDL_Point* getCenter();  
		};
	}
}