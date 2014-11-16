/*************************************************************************
Protheus Source File.
Copyright (C), Protheus Studios, 2013-2014.
-------------------------------------------------------------------------

Description:
Contains data about a specific tile, such as it's size, physical
properites and the sprite assigned to the tile.
-------------------------------------------------------------------------
History:
- 20:05:2014: Waring J.
*************************************************************************/

#pragma once

#include <SDL.h>
#include <vector>
#include <string>
#include "..\lua\LuaUtils.h"

namespace Pro{
	namespace GameObject{
		struct TileType
		{
			unsigned short tileID;
			bool passable = true;

			bool isTextured = false;
			std::string spriteName;
			void* spriteCache = nullptr;

			/*constexpr*/ static const char* lGetMetatable(){
				return "tile_type_metatable";
			}

			template<typename T>
			static inline void lGetFunctions(std::vector<luaL_Reg>& fields){
			}
		};
	}
}