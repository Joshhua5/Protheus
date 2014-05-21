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
#include "Sprite.h"
#include "Scene.h"
#include <unordered_map>
#include "SpriteManager.h"
#include <SDL.h>
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
			Renderer();
			~Renderer();
			 
			SDL_Renderer* getRenderer();

			void renderScene(Scene::Scene*, Graphics::SpriteManager*);
		};
	}
}