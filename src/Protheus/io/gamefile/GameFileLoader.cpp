#include "GameFileLoader.h"

using namespace Pro;
using namespace IO;



void inline loadIndex(CFile* file, unordered_map<string, unsigned int>& index, const EChunkType type){
	GameFileChunk c_chunk;
	c_chunk.chunkName = move(file->read<char>(32));
	c_chunk.chunkType = type;
	c_chunk.chunkData = move(file->read(file->read<unsigned>()));

	GameFileIndex index_factory(c_chunk);
	index = move(index_factory.extractIndex());
}

void GameFileLoader::load(const string& file_path){
	file = new CFile(file_path);  
	loadIndex(file, image_index,	EChunkType::IMAGE_INDEX);
	loadIndex(file, script_index,	EChunkType::SCRIPT_INDEX);
	loadIndex(file, audio_index,	EChunkType::AUDIO_INDEX);
	loadIndex(file, object_index,	EChunkType::OBJECT_INDEX);
	loadIndex(file, map_index,		EChunkType::MAP_INDEX);
	loadIndex(file, map_index,		EChunkType::MAP_TILE_INDEX);
	loadIndex(file, raw_index,		EChunkType::RAW_INDEX); 	 
}

const char* GameFileLoader::getScript(const string& name){ 
	file->setReadPosition(script_index.at(name)); 
	const auto script_size = file->read<unsigned>();
	return file->read<char>(script_size);
}

const char* GameFileLoader::getConfigScript(){
	return getScript("config.lua");
}

game_id GameFileLoader::getImage(lua_State* L, const string& name){ 
	GameFileChunk chunk;
	file->setReadPosition(image_index.at(name));
	chunk.chunkName = name;
	chunk.chunkData = file->read(file->read<unsigned>());

	GameFileImage image(L, chunk);
	return image.getSpriteID();
}

CBuffer GameFileLoader::getAudio(const string& name){
	// find the offset of the audio file and set the read position
	file->setReadPosition(audio_index.at(name));
	// read the first 4 bytes for the size of the buffer
	// read the whole buffer and return it
	return file->read(file->read<unsigned>()); 
}


Map* GameFileLoader::getMap(const string& name){ 
	// load in the chunk
	GameFileChunk chunk;
	file->setReadPosition(map_index.at(name));
	chunk.chunkName = name;
	chunk.chunkData = file->read(file->read<unsigned>());

	GameFileMap map(chunk);
	map.unpack(chunk);
	return map.getMap();
}


CBuffer GameFileLoader::getRaw(const string& name){
	file->setReadPosition(raw_index.at(name));
	return file->read(file->read<unsigned>());
}