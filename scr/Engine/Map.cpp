#include "Map.h"

using namespace Pro;
using namespace Scene;

Map::Map()
{
}


Map::~Map()
{
}
inline std::string getString(const std::string& line){
	return line.substr(line.find(':') + 1, line.find(';') - line.find(':') - 1);
}
  
bool Map::loadLevel(const std::string& file, const std::string& data){
	if (loadLevelData(file) &&
		loadLevelTileData(data))
		return true;
	return false;
}

bool Map::loadLevelData(const std::string& file){
	std::fstream stream(file.c_str(), std::fstream::in | std::fstream::binary);
	std::vector<std::vector<char>> data;

	if (stream.bad() == true){
		stream.close();
		std::string err = "Failed to load level data " + file;
		SDL_LogError(SDL_LOG_CATEGORY_ERROR, err.c_str());
	}

	const unsigned int bufferSize = 9068;
	char* buffer = new char[bufferSize];
	while (stream.getline(buffer, bufferSize) && buffer[0] != ';'){
		// read a horizontal line 
		std::vector<char> horzLine; 
		for (int x = 0; buffer[x] != '\0'; x++)
			horzLine.push_back(buffer[x]); 
		data.push_back(horzLine);
	}
	dimensions = Math::Vector2(data[0].size(), data.size());  

	if (stream.bad() == true){
		stream.close();
		std::string err = "Failed to load level, Phase 2: " + file;
		SDL_LogError(SDL_LOG_CATEGORY_ERROR, err.c_str());
	}

	// Load Map Section Data
	// If data isn't in a section, it doesn't get loaded

	while (stream.getline(buffer, bufferSize) && stream.eof() == false){
		MapSection* mapSec = new MapSection();
		std::vector<std::vector<char>> mapSecData;
		Math::Vector4 rect = Util::stringToVec4(buffer);
		for (int x = static_cast<int>(rect.x); x < static_cast<int>(rect.w); x++){
			std::vector<char> dat;
			for (int y = static_cast<int>(rect.y); y < static_cast<int>(rect.w); y++)
				dat.push_back(data.at(y).at(x));
			mapSecData.push_back(dat);
		}
		mapSec->setData(mapSecData);
		mapSections.push_back(mapSec);
	}
delete[] buffer;
	stream.close();
	return true;
}

bool Map::loadLevelTileData(const std::string& file){
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
			currentID = Util::stringToInt(line.substr(1, x - 1));
			tileData.at(currentID).tileID = currentID;
			continue;
		}
		// check first word

		std::string key = line.substr(0, line.find_first_of(':'));

		switch (key[0]){
		case 'p':
			if (key == "passable"){
				tileData[currentID].passable = Util::stringToBoolean(getString(line));
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
				tileData[currentID].tileSize.w = Util::stringToInt(getString(line));
			}
			break;
		case 'h':
			if (key == "height"){
				tileData[currentID].tileSize.h = Util::stringToInt(getString(line));

			}
		}
	}
	tileData.shrink_to_fit();
	stream.close();
	return true;
}

TileType* Map::getTile(Math::Vector2 v){
	char* ch = nullptr;
	for each(const auto &section in mapSections)
		if ((ch = section->contains(v)) != nullptr)
			return &tileData.at(*ch);
	return nullptr;
}

std::vector<MapSection*> Map::getVisibleSections(Camera* cam){
	std::vector<MapSection*> sections;
	for each(const auto &sec in mapSections)
		if (sec->visible(cam))
			sections.push_back(sec);
	return sections;
}
   
TileType* Map::getTileType(unsigned int ch){
	return &tileData.at(ch);
}
