#include "GameFileMap.h"


using namespace Pro;
using namespace IO;
using namespace Math;
using namespace GameObject;


GameFileMap::GameFileMap(Map* map){ store(map);} 
GameFileMap::GameFileMap(){}  
GameFileMap::~GameFileMap(){}

// stores a map into a gamefile chunk
void GameFileMap::store(Map* map){ 
	  
	// populate and define the chunk
	dataChunk.chunkType = EChunkType::MAP_DATA;
	dataChunk.chunkID = -1;

	// define the chunk size and 
	// initialize the dataChunk

	int chunkDataSize = 0;
	// first int that's used for the section count
	chunkDataSize += sizeof(int);
	// size for section definitions
	chunkDataSize += map->getSectionCount() * (sizeof(int) + sizeof(Vector2) + sizeof(Vector2));
	// size for the section data
	chunkDataSize += map->getVolume() * sizeof(short);
	dataChunk.chunkData.init(chunkDataSize);
	    
	// create Bufferwriters for the chunk
	BufferWriter dataWriter(&dataChunk.chunkData); 

	// write the chunk header 
	int count = map->getSectionCount();
	dataWriter.write(&count, sizeof(int)); 

	// writes the sections into the data buffer
	for each(auto section in map->getSections()){
		dataWriter.write(static_cast<int>(section->getVolume()));
		dataWriter.write<Vector2>(section->getPosition());
		dataWriter.write<Vector2>(section->getDimensions()); 

		for each(auto col in section->getData())
			for each(auto row in col)
				dataWriter.write(row); 
	} 
} 

// loads a map from a gamefile chunk
GameObject::Map* GameFileMap::load(){
	auto map = new Map();

	BufferReader dataReader(&dataChunk.chunkData);

	int sectionCount = dataReader.read<int>();
	  
	// Process each section
	for (int x = 0; x < sectionCount; x++){
		MapSection section; 
		
		// In the buffer each section is as follows
		// 1 sectionSize : int
		// 2 position : vector2
		// 3 dimensions : vector2
		// 4 sectionData : short * sectionSize
		// next section

		auto sectionSize = dataReader.read<int>(); 
		auto sectionPosititon = dataReader.read<Vector2>(); 
		auto sectionDimension = dataReader.read<Vector2>();
		
		section.setPosition(sectionPosititon);
		section.setDimensions(sectionDimension); 

		// load in the section data from a 1D short array
		// to a 2D short vector
		vector<vector<short>> sectionData(sectionDimension.x); 
		for (auto x = 0; sectionDimension.x; x++){ 
			vector<short> rowData(sectionDimension.y);

			for (auto y = 0; sectionDimension.y; y++)
				rowData.push_back(dataReader.read<short>());

			// we move because we'll never need rowData again
			// so it's safe not to duplicated it into the array
			sectionData.push_back(move(rowData));
		}
		section.setData(sectionData);
		// add section to the map
		map->addSection(section);
	}  

	return map;
} 