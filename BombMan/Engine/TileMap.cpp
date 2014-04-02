#include "TileMap.h"
namespace Pro{

	void TileMap::loadLevelData(const std::string& file){
		std::fstream stream(file.c_str(), std::fstream::in | std::fstream::binary);

		if (stream.bad() == true){
			stream.close();
			std::string err = "Failed to load level data " + file;
			SDL_LogError(SDL_LOG_CATEGORY_ERROR, err.c_str());
		}

		const unsigned int bufferSize = 9068;
		char* buffer = new char[bufferSize];
		while (stream.getline(buffer, bufferSize) && stream.eof() == false){
			// read a horizontal line
			std::vector<char> horzLine;
			int x = 0;
			for (; buffer[x] != '\0'; x++)
				horzLine.push_back(buffer[x]);
			width = x;
			data.push_back(horzLine);
		}
		width = data[0].size();
		height = data.size();
		delete [] buffer;

		if (stream.bad() == true){
			stream.close();
			std::string err = "Failed to load level, Phase 2: " + file;
			SDL_LogError(SDL_LOG_CATEGORY_ERROR, err.c_str());
		}
		stream.close();
	}
	std::string TileMap::getString(const std::string& line){
		return line.substr(line.find(':') + 1, line.find(';') - line.find(':') - 1);
	}

	bool TileMap::getBoolean(const std::string& line){  
		if (line.substr(line.find(':') + 1, line.find(';') - line.find(':') - 1) == "true")
			return true;
		return false;
	}

	void TileMap::loadLevelTileData(const std::string& file){
		std::fstream stream(file.c_str(), std::fstream::in | std::fstream::binary);
		tileData.resize(20);

		if (stream.bad() == true){
			stream.close();
			std::string err = "Failed to load tile data " + file;
			SDL_LogError(SDL_LOG_CATEGORY_ERROR, err.c_str());
		}
		std::string line;
		line.resize(512);
		unsigned int currentID = 0;
		while (stream.eof() != true){
			stream.getline(&line[0], line.size());
			if (line[0] == '['){
				// mark finish of previous tile decleration
				// Get Tile ID
				unsigned char x = 0;
				while (line[x] != ']')
					x++;
				currentID = atoi(line.substr(1, x - 1).c_str());
				tileData.at(currentID).tileID = currentID;
				continue;
			}
			// check first word

			std::string key = line.substr(0, line.find_first_of(':'));

			switch (key[0]){
			case 'p':
				if (key == "passable"){
					tileData[currentID].passable = getBoolean(line);
				}
				break;
			case 's':
				if (key == "sprite"){
					tileData[currentID].spriteName = getString(line);
					tileData[currentID].isTextured = true;
				}
				break;
			case 'w':
				if (key == "width"){
					tileData[currentID].tileSize.w = atoi(getString(line).c_str());
				}
				break;
			case 'h':
				if (key == "height"){
					tileData[currentID].tileSize.h = atoi(getString(line).c_str());

				}
			}
		}
		tileData.shrink_to_fit();
		stream.close();
	}

	void TileMap::loadLevel(const std::string& file, const std::string& dataFile){
		if (file != "")
			loadLevelData(file);
		if (dataFile != "")
			loadLevelTileData(dataFile);
	}


	TileType* TileMap::getTile(unsigned int x, unsigned int y){
		return &tileData[data[x][y]];

	}
}