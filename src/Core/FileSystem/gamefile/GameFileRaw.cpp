#include "GameFileRaw.h"

using namespace Pro;
using namespace IO;
using namespace std;

GameFileRaw::GameFileRaw(const string& name, CBuffer* buffer){
	pack(name, buffer);
}
GameFileRaw::GameFileRaw(GameFileChunk& chunk){
	unpack(chunk);
}

void GameFileRaw::pack(const string& name, CBuffer* buffer){
	m_chunk.chunkData = *buffer;
	m_chunk.chunkName = name;
	m_chunk.chunkType = EChunkType::RAW;
}

void GameFileRaw::pack(string&& name, CBuffer* buffer){
	m_chunk.chunkData = *buffer;
	m_chunk.chunkName = move(name);
	m_chunk.chunkType = EChunkType::RAW;
}

void GameFileRaw::unpack(GameFileChunk& _chunk){
	data = &_chunk.chunkData;
}

CBuffer* GameFileRaw::getData(){
	return data;
}