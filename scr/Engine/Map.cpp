#include "Map.h"

using namespace Pro;
using namespace Scene;

Map::Map()
{
}


Map::~Map()
{
}
std::string Map::getString(const std::string& line){
	return line.substr(line.find(':') + 1, line.find(';') - line.find(':') - 1);
}

bool Map::getBoolean(const std::string& line){
	if (line.substr(line.find(':') + 1, line.find(';') - line.find(':') - 1) == "true")
		return true;
	return false;
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
		int x = 0;
		for (; buffer[x] != '\0'; x++)
			horzLine.push_back(buffer[x]);
		dimensions.w = x;
		data.push_back(horzLine);
	}
	dimensions.w = data[0].size();
	dimensions.h = data.size();
	

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
		SDL_Rect rect = getRect(buffer);
		for (int x = rect.x; x < rect.w; x++){
			std::vector<char> dat;
			for (int y = rect.y; y < rect.h; y++)
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
	return true;
}

TileType* Map::getTile(unsigned int x, unsigned int y){
	char* ch = nullptr;
	for each(const auto &section in mapSections)
		if ((ch = section->contains(x, y)) != nullptr)
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

SDL_Rect Map::getRect(const std::string& line){
	SDL_Rect out;
	auto pos = line.find(' ');
	auto pos1 = line.find(' ', pos + 1);
	out.x = atoi(line.substr(0, pos).c_str());
	out.y = atoi(line.substr(pos + 1, pos1 - pos).c_str());
	pos = line.find(' ', pos1 + 1);
	out.w = atoi(line.substr(pos1 + 1, pos - pos1).c_str());
	out.h = atoi(line.substr(pos + 1, line.length() - pos).c_str());
	return out;
}


TileType* Map::getTileType(char ch){
	return &tileData.at(ch);
}
