#include "GameFileMap.h"

using namespace Pro;
using namespace IO;
using namespace Math;
using namespace GameObject;
using namespace Util;
 

// stores a map into a gamefile chunk
void GameFileMap::pack(Map* map){
	// populate and define the chunk
	m_chunk.chunkType = EChunkType::MAP_DATA;
	m_chunk.chunkName = GUIDLookup::getName(map->getGUID());

	// define the chunk size and
	// initialize the dataChunk

	int chunkDataSize = 0;
	// first int that's used for the section count
	chunkDataSize += sizeof(int);
	// size for section definitions
	chunkDataSize += map->getSectionCount() * (sizeof(int) + sizeof(Vector2) + sizeof(Vector2));
	// size for the section data
	chunkDataSize += static_cast<int>(map->getVolume() * sizeof(short));
	m_chunk.chunkData.init(chunkDataSize);

	// create Bufferwriters for the chunk
	BufferWriter dataWriter(&m_chunk.chunkData);

	// write the chunk header
	int count = map->getSectionCount();
	dataWriter.write(&count, sizeof(int));

	// writes the sections into the data buffer
	for each(const auto section in map->getSections()){
		dataWriter.write<Vector2>(section->getPosition());
		dataWriter.write<Vector2>(section->getDimensions());

		for each(auto col in section->getData())
			for each(auto row in col)
				dataWriter.write(row);
	}
}

// loads a map from a gamefile chunk
void GameFileMap::unpack(GameFileChunk& chunk){
	m_map = new Map();

	BufferReader dataReader(&chunk.chunkData);

	const auto sectionCount = dataReader.read<int>();

	// Process each section
	for (int sectionID = 0; sectionID < sectionCount; sectionID++){
		auto section = new MapSection();

		// In the buffer each section is as follows
		// 1 sectionSize : int
		// 2 position : vector2
		// 3 dimensions : vector2
		// 4 sectionData : short * sectionSize
		// next section

		auto sectionPosititon = dataReader.read<Vector2>();
		auto sectionDimension = dataReader.read<Vector2>();

		section->setPosition(sectionPosititon);
		section->setDimensions(sectionDimension);

		// load in the section data from a 1D short array
		// to a 2D short vector
		vector<vector<short>> sectionData(static_cast<int>(sectionDimension.x));
		for (auto x = 0; sectionDimension.x; ++x){
			vector<short> rowData(static_cast<int>(sectionDimension.y));

			for (auto y = 0; sectionDimension.y; y++)
				rowData.push_back(dataReader.read<short>());

			// we move because we'll never need rowData again
			// so it's safe not to duplicated it into the array
			sectionData.push_back(move(rowData));
		}
		section->setData(sectionData);
		// add section to the map
		m_map->addSection(section);
	}
}

Map* GameFileMap::getMap(){
	return m_map;
}