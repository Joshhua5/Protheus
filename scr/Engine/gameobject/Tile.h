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
namespace Pro{
	namespace GameObject{
		struct TileType
		{
			SDL_Rect tileSize;
			char tileID;
			bool passable = true;

			bool isTextured = false;
			std::string spriteName;
			void* spriteCache = nullptr;
		};
	}
}