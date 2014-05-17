#pragma once
#include <SDL.h>
#include <string>
#include <fstream>
#include <vector>

#include "Camera.h"
#include "Tile.h"
#include "MapSection.h"

// Build to replace TileMap

// Theory
/**
*	The world map is broken down into sections
*	Each section is defined according to the datafile
*	each sections has a active variable and will only update
*	if the section is active
*/
namespace Pro{
	namespace Scene{
		class Map
		{
		private:
			SDL_Rect dimensions;

			std::vector<MapSection*> mapSections;
			std::vector<TileType> tileData;
			bool getBoolean(const std::string& line);
			std::string getString(const std::string& line);
			SDL_Rect getRect(const std::string& line);

			bool loadLevelData(const std::string& file);
			bool loadLevelTileData(const std::string&file);
		public:
			Map();
			~Map();

			std::vector<MapSection*> getVisibleSections(Camera* cam);
			bool loadLevel(const std::string& file, const std::string& data);
			TileType* getTile(unsigned int _x, unsigned int _y);
			TileType* getTileType(char);
		};
	} 
}