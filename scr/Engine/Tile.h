#pragma once 
#include "Sprite.h"
#include <SDL_rect.h>
#include <vector> 
namespace Pro{

	struct TileType
	{
		SDL_Rect tileSize;
		char tileID;
		bool passable = true; 

		bool isTextured = false;
		std::string spriteName;
		Sprite* sprite = nullptr; 
		 
	}; 
}