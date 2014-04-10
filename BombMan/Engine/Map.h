#pragma once
#include <SDL_rect.h>
#include <string>
#include "Camera.h"
#include <fstream>
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
		char* contains( int,  int);
		SDL_Rect getDimensions();
		void setData(std::vector<std::vector<char>>);
		std::vector<std::vector<char>> getData();
		bool visible(Camera* cam);
	};
	 
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
 