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
#include <string>
#include <vector> 
#include <unordered_map>
#include <thread>
#include <fstream>
#include <SDL_image.h>
#include <SDL.h> 
#include "AnimatedSprite.h"  
#include "LuaUtils.h"

namespace Pro{
	namespace Graphics{
		class SpriteManager
		{
			SDL_Texture* spriteSheet;
			std::unordered_map<std::string, Asset::Sprite> sprites;
			std::unordered_map<std::string, Asset::AnimatedSprite> animations;

			Asset::AnimatedSprite loadAnimation(SDL_Renderer *renderer, const std::string& path);
		public:
			SpriteManager();
			~SpriteManager();

			Asset::Sprite* getSprite(const std::string &_name);
			Asset::AnimatedSprite* getAnim(const std::string& name);
			SDL_Texture* getSpriteSheet();

			bool loadSpriteSheet(SDL_Renderer *renderer, const std::string& image, const std::string& data);
			bool loadAnimations(SDL_Renderer* renderer, const std::string& path);
			void release();

			// LUA Functions
			static int lLoadSpriteSheet(lua_State* L);
			static int lLoadSpriteAnimations(lua_State* L);
		};
	}
}