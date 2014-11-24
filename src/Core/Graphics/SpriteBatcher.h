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

#include <stack>
#include <Error.h>
#include <Vector4.h> 
#include "Sprite.h"

namespace Pro{
	namespace Graphics {
		using namespace std;
		using namespace Asset;
		using namespace Math;

		class SpriteBatcher
		{
			struct Details{
				float scale;
				float rotation;
				Sprite* sprite;
				Vector4 position;
			};  
			SDL_Renderer* renderer;
			stack<Details> render_stack; 
		public:
			SpriteBatcher(SDL_Renderer*);
			 
			void push(Sprite*, Vector4&, float scale = 1, float rotate = 0);
			void flush(); 
		};
	}
}
