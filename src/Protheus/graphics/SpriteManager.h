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
#include "..\io\CFile.h"
#include "..\io\FileSystem.h"
#include "..\util\CBuffer.h"
#include "..\util\LuaUtils.h"

namespace Pro{
	namespace Graphics{
		using namespace Asset;

		class SpriteManager
		{
			lua_State* lua_state;
			SDL_Renderer* renderer = nullptr;
			SDL_Texture* spriteSheet;
			std::unordered_map<game_id, Sprite*> sprites;
			std::unordered_map<game_id, AnimatedSprite*> animations;

			AnimatedSprite loadAnimation(const std::string& path);
		public:
			SpriteManager(lua_State* lua_state);
			~SpriteManager();

			Sprite* getSprite(game_id);
			AnimatedSprite* getAnim(game_id);

			Sprite* loadSprite(const std::string& name, const string& imagePath);
			void release(game_id);

			// LUA Functions
			static int lLoadSprite(lua_State*);
			static int lGetSprite(lua_State*);
			static int lGetAnimation(lua_State*);

			/*constexpr*/ static const char* lGetMetatable(){
				return "sprite_manager_metatable";
			}

			template<typename T>
			static void lGetFunctions(std::vector<luaL_Reg>& fields){
				fields.push_back({ "loadSprite", &T::lLoadSprite });
				fields.push_back({ "getSprite", &T::lGetSprite });
				fields.push_back({ "getAnimation", &T::lGetAnimation });
			}
		};
	}
}