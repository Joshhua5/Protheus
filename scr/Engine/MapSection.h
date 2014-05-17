#pragma once
#include <SDL_rect.h>
#include <vector> 
#include "Tile.h"
#include "Camera.h"

namespace Pro{
	namespace Scene{

		class MapSection{
			SDL_Rect dimensions;
			std::vector<std::vector<char>> data;
		public:
			char tileAt(unsigned int, unsigned int);
			char* contains(int, int);
			SDL_Rect getDimensions();
			void setData(std::vector<std::vector<char>>);
			std::vector<std::vector<char>> getData();
			bool visible(Camera* cam);
		};
	}
}