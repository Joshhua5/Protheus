#include "Map.h"

using namespace Pro;
using namespace GameObject;
using namespace std;
using namespace Math;

Map::Map()
{
}

inline string getString(const string& line){
	return line.substr(line.find(':') + 1, line.find(';') - line.find(':') - 1);
}

TileType* Map::getTile(Vector2& v){
	short* ch = nullptr;
	for each(const auto &section in mapSections)
		if ((ch = section->contains(v)) != nullptr)
			return &tileData.at(*ch);
	return nullptr;
}

vector<MapSection*> Map::getVisibleSections(DataEntity& cam){
	vector<MapSection*> sections;
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