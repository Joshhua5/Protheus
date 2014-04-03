#pragma once
#include <SDL_rect.h>
#include <string>
#include <vector>
#include "Tile.h"
// Build to replace TileMap

// Theory
/**
*	The world map is broken down into sections
*	Each section is defined according to the datafile
*	each sections has a active variable and will only update
*	if the section is active
*/
namespace Pro{
	class MapSection{
		SDL_Rect dimensions;
		std::vector<std::vector<char>> data;
	public:
		char tileAt(unsigned int, unsigned int);
		SDL_Rect getDimensions();
	};


	class Map
	{
	private:
		SDL_Rect dimensions;
		 
		std::vector<MapSection*> mapSections;
		std::vector<TileType> tileData;
		bool getBoolean(const std::string& line);
		std::string getString(const std::string& line); 

		void loadLevelData(const std::string& file);
		void loadLevelTileData(const std::string&file);
	public:
		Map();
		~Map();
		
		void loadLevel(const std::string& file, const std::string& data); 
	};
}
 