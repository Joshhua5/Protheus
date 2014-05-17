#pragma once
#include <stack>
#include "Math.h"
#include "Sprite.h"


namespace Pro{
	namespace Graphics {
		class SpriteBatcher
		{ 
			SDL_Renderer* renderer;
			std::stack<Asset::Sprite*> sprite_stack;
			std::stack<Math::Vector4> rect_stack;
		public:
			SpriteBatcher(SDL_Renderer*);
			SpriteBatcher();
			~SpriteBatcher();

			void push(Asset::Sprite*, Math::Vector4);
			void flush();
		};
	}
}
