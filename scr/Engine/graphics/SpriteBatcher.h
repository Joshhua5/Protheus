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

#include "..\Math.h"
#include "Sprite.h"


namespace Pro{
	namespace Graphics {
		class SpriteBatcher
		{ 
			lua_State* lua_state;
			SDL_Renderer* renderer;
			std::stack<Asset::Sprite*> sprite_stack;
			std::stack<Math::Vector4> rect_stack;
		public:
			SpriteBatcher(lua_State* lua_state);
			SpriteBatcher();
			~SpriteBatcher();

			void push(Asset::Sprite*, Math::Vector4&);
			void flush();
		};
	}
}
