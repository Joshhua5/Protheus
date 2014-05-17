#pragma once  
#include <SDL_rect.h>
#include <vector> 
namespace Pro{
	namespace Scene{
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