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

#include "..\gameobject\Scene.h"
#include <unordered_map>
#include "SpriteManager.h"
#include "SpriteBatcher.h"
namespace Pro{
	namespace Graphics{
		class Renderer
		{
		private:
			lua_State* lua_state;
			SDL_Renderer* renderer;
			SpriteBatcher* sprite_batcher;
		public:
			Renderer(lua_State* lua_state); 
			~Renderer();

			SDL_Renderer* getRenderer();

			void renderScene(Scene*, Graphics::SpriteManager*);

			// Lua Functions

			static int lGetBatcher(lua_State*);
			static int lRenderScene(lua_State*);

			static inline string lGetMetatable(){
				return "renderer_metatable";
			}

			template<typename T>
			static void lGetFunctions(std::vector<luaL_Reg>& fields){
				fields.push_back({ "getBatcher", (lua_CFunction)&T::lGetBatcher });
				fields.push_back({ "renderScene", (lua_CFunction)&T::lRenderScene });
			}
		};
	}
}