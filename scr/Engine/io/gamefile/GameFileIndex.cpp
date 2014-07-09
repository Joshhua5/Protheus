#include "GameFileIndex.h"

using namespace Pro;
using namespace IO;

GameFileIndex::GameFileIndex(GameFileChunk& chunk){
	unpack(chunk);
}

void GameFileIndex::pack(vector<pair<string, unsigned int>> indexes, EChunkType type){
	m_chunk.chunkName = "";
	m_chunk.chunkType = type;

	auto structSize = 0;
	structSize += sizeof(unsigned int);
	structSize += sizeof(char) * 32;

	m_chunk.chunkData.init(structSize * indexes.size());
	Util::BufferWriter writer(&m_chunk.chunkData);

	for each(auto index in indexes){
		writer.write<unsigned int>(index.second);
		writer.write_array<char>(&index.first[0], 32);
	}
}

void GameFileIndex::unpack(GameFileChunk& _chunk){
	auto structSize = 0;
	structSize += sizeof(unsigned int);
	structSize += sizeof(char) * 32;

	auto indexCount =
		_chunk.chunkData.size / structSize;

	Util::BufferReader reader(&_chunk.chunkData);

	for (size_t x = 0; x < indexCount; ++x){
		pair<string, unsigned int> index;
		index.second = reader.read<unsigned int>();
		index.first = reader.read_array<char>(32);
		m_indexes.insert(index);
	}
}

unsigned int GameFileIndex::getOffset(const string& name){
	return m_indexes.at(name);
}