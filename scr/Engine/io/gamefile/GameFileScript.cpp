
#include "GameFileScript.h"

using namespace Pro;
using namespace IO;

GameFileScript::GameFileScript(GameFileChunk)
{
}

 

char* GameFileScript::getScript(){
	return m_script;
}

bool GameFileScript::executeScript(lua_State* L){
	return luaL_dostring(L, m_script);
}

void GameFileScript::unpack(GameFileChunk& _script){
	m_script = static_cast<char*>(_script.chunkData.data);
	// prevent out script from being deleted
	_script.chunkData.dereference();
}

void GameFileScript::pack(const string& _script, const string& name){ 
	m_chunk.chunkName = name;
	m_chunk.chunkType = EChunkType::SCRIPT;
	m_chunk.chunkData.init((void*) _script.c_str(), _script.length());

}