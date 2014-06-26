#include "GameFileMap.h"


using namespace Pro;
using namespace IO;
 

GameFileMap::GameFileMap()
{

}


GameFileMap::~GameFileMap()
{
}


// stores a map into a gamefile chunk
void GameFileMap::store(GameObject::Map* map){ 
	
	// count how many tiles are in the map
	auto condensedMap = map->condenseMap();

	unsigned int totalTiles = 
		condensedMap->getDimensions().x *
		condensedMap->getDimensions().y;

	// use how many tiles there are to determin if we need
	// a byte or a short

	unsigned char byte_per_tile;
	if (map->getTileData().size() <= 255)
		byte_per_tile = 1;
	else
		byte_per_tile = 2;
	
	// populate and define the chunk

	chunk.chunkType = EChunkType::MAP;
	chunk.chunkID = -1;
	chunk.chunkData.init(1 + (totalTiles * byte_per_tile));

	// fill the chunk's data with tile data

	CBufferWriter writer(chunk.chunkData);
	// write the byte_per_tile
	writer.write(&byte_per_tile, 1);

	auto tileData = condensedMap->getData();
	for (int x = condensedMap->getPosition().x; x < condensedMap->getDimensions().x; x++){
		for (int y = condensedMap->getPosition().y; y < condensedMap->getDimensions().y; y++){
			// writes the tile data to the buffer
			writer.write(&tileData.at(x).at(y), byte_per_tile); 
		} 
	}
}
// loads a map from a gamefile chunk
GameObject::Map* GameFileMap::load(){
	return nullptr;
}