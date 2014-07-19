#include "GameFileScript.h"

using namespace Pro;
using namespace IO;

GameFileScript::GameFileScript(GameFileChunk)
{
}

char* GameFileScript::getScript(){
	return m_script;
}

void GameFileScript::unpack(GameFileChunk& _script){
	m_script = _script.chunkData.data<char>();
	// prevent out script from being deleted
	_script.chunkData.dereference();
}

void GameFileScript::pack(const string& _script, const string& chunk_name){
	m_chunk.chunkName = chunk_name;
	m_chunk.chunkType = EChunkType::SCRIPT;
	m_chunk.chunkData.init((void*)_script.data(), _script.length(), true);

	// Saved in the file as
	// buffer Size
	// buffer

	// index contains the name and type is used to 
	// determine what index it'll be inside.
}