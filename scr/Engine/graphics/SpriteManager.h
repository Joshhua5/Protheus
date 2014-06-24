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
#include "..\graphics\AnimatedSprite.h"  
#include "..\util\LuaUtils.h"

namespace Pro{
	namespace Graphics{
		class SpriteManager
		{
			SDL_Texture* spriteSheet;
			std::unordered_map<uint32, Asset::Sprite> sprites;
			std::unordered_map<uint32, Asset::AnimatedSprite> animations;

			Asset::AnimatedSprite loadAnimation(SDL_Renderer *renderer, const std::string& path);
		public:
			SpriteManager();
			~SpriteManager();

			Asset::Sprite* getSprite(uint32);
			Asset::AnimatedSprite* getAnim(uint32);
			SDL_Texture* getSpriteSheet();

			bool loadSpriteSheet(SDL_Renderer *renderer, const std::string& image, const std::string& data);
			bool loadAnimations(SDL_Renderer* renderer, const std::string& path);
			void release();

			// LUA Functions
			static int lLoadSpriteSheet(lua_State* L);
			static int lLoadSpriteAnimations(lua_State* L); 
			static int lGetSprite(lua_State*);
			static int lGetAnimation(lua_State*);

			static inline string lGetMetatable(){
				return "sprite_manager_metatable";
			}

			template<typename T>
			static void lGetFunctions(std::vector<luaL_Reg>& fields){
				fields.push_back({ "loadSpriteSheet", &T::lLoadSpriteSheet });
				fields.push_back({ "loadSpriteAnimations", &T::lLoadSpriteAnimations });
				fields.push_back({ "getSprite", &T::lGetSprite });
				fields.push_back({ "getAnimation", &T::lGetAnimation });
			}
		};
	}
}