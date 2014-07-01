
#include "Map.h"

using namespace Pro;
using namespace GameObject;

Map::Map()
{
}


Map::~Map()
{
}
inline std::string getString(const std::string& line){
	return line.substr(line.find(':') + 1, line.find(';') - line.find(':') - 1);
}
 
TileType* Map::getTile(Math::Vector2& v){
	short* ch = nullptr;
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


void Map::addSection(MapSection* section){
	mapSections.push_back(section);
}

unsigned int Map::getSectionCount(){
	return mapSections.size();
}