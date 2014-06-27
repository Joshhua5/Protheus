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

	// use how many tiles there are to determin if we need
	// a byte or a short

	unsigned char byte_per_tile = 
		map->getTileData().size() <= 255 ? 1 : 2;  

	// populate and define the chunk
	dataChunk.chunkType = EChunkType::MAP_DATA;
	dataChunk.chunkID = -1;
	dataChunk.chunkData.init(condensedMap->getVolume() * byte_per_tile);
	 
	headerChunk.chunkType = EChunkType::MAP_HEADER;
	headerChunk.chunkID = -1;
	headerChunk.chunkData.init(1 + (sizeof(Math::Vector2) * 2));

	// create Bufferwriters for the chunks

	CBufferWriter dataWriter(dataChunk.chunkData);
	CBufferWriter headerWriter(headerChunk.chunkData);

	// write the chunk header
	headerWriter.write(&byte_per_tile, 1);
	headerWriter.write(&condensedMap->getPosition(), sizeof(Math::Vector2));
	headerWriter.write(&condensedMap->getDimensions(), sizeof(Math::Vector2));

	// write all tiles into the buffer
	for each(auto col in condensedMap->getData())
		for each (auto field in col)
			dataWriter.write(&field, byte_per_tile); 
}

// loads a map from a gamefile chunk
GameObject::Map* GameFileMap::load(){
	return nullptr;
}