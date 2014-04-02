#include "Tile.h"

#include <string> 
#include <vector>
#include <fstream>
#include <chrono>
#include <SDL.h>
namespace Pro{

	struct TileMap{
		long width, height;
		std::vector<std::vector<char>> data;
		std::vector<TileType> tileData;
		// returns a x and y coord in a array

		// pass NULL to skip loading a file
		void loadLevel(const std::string& levelDataPath, const std::string& tileDataPath);
		TileType* getTile(unsigned int x, unsigned int y);
	private:
		void loadLevelData(const std::string& file);
		void loadLevelTileData(const std::string& file);

		bool getBoolean(const std::string& line);
		std::string getString(const std::string& line);
	}; 
}