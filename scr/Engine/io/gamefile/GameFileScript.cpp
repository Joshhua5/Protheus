
#include "GameFileScript.h"

using namespace Pro;
using namespace IO;

GameFileScript::GameFileScript(GameFileChunk)
{
}

 

char* GameFileScript::getScript(){
	return script;
}

bool GameFileScript::executeScript(lua_State* L){
	return luaL_dostring(L, script);
}

void GameFileScript::unpack(GameFileChunk& _script){
	script = static_cast<char*>(_script.chunkData.data);
	// prevent out script from being deleted
	_script.chunkData.dereference();
}

void GameFileScript::pack(const string& _script, const string& name){ 
	chunk.chunkName = name;
	chunk.chunkType = EChunkType::SCRIPT;
	chunk.chunkData.init((void*)_script.c_str(), _script.length());

}