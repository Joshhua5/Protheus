#pragma once
#include "Vectors.h"
#include "Sprite.h"
#include <stack>

namespace Pro{
	namespace Graphics {
		class SpriteBatcher
		{ 
			SDL_Renderer* renderer;
			std::stack<Sprite*> sprite_stack;
			std::stack<SDL_Rect> rect_stack;
		public:
			SpriteBatcher(SDL_Renderer*);
			SpriteBatcher();
			~SpriteBatcher();

			void push(Sprite*, SDL_Rect);
			void flush();
		}; 
	}
}
