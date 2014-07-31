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

#include "..\math\Vector4.h"
#include "..\util\LuaUtils.h"
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
			lua_State* lua_state;
			SDL_Renderer* renderer;
			stack<Details> render_stack; 
		public:
			SpriteBatcher(lua_State* lua_state); 

			void push(Sprite*, Vector4&);
			void push(Sprite*, Vector4&, float scale);
			void push(Sprite*, Vector4&, float scale, float rotate);
			void flush();

			// Lua Functions

			static int lPush(lua_State*);
			static int lFlush(lua_State*);

			/*constexpr*/ static const char* lGetMetatable(){
				return "sprite_batcher_metatable";
			}

			template<typename T>
			static void lGetFunctions(std::vector<luaL_Reg>& fields){
				fields.push_back({ "push", &T::lPush });
				fields.push_back({ "flush", &T::lFlush });
			}
		};
	}
}
