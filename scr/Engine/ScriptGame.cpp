#include "ScriptGame.h"

using namespace Pro;


ScriptGame::ScriptGame(lua_State* L){
	lua_state = L;
}

ScriptGame::ScriptGame(){
	lua_state = nullptr;
}


ScriptGame::~ScriptGame(){}


int ScriptGame::update(){  
	lua_getglobal(lua_state, "Update");
	if (lua_pcall(lua_state, 0, 0, 0) != 0)
		return -1; 
	return 0;
}

int ScriptGame::gameLoop(){ 
	do{
		update();
		render();
		lua_getglobal(lua_state, "isExitRequested");
	} while (lua_toboolean(lua_state, -1));
	return 0;
}

int ScriptGame::render(){
	lua_getglobal(lua_state, "Render");
	if (lua_pcall(lua_state, 0, 0, 0) != 0)
		return -1;
	return 0;
}
int ScriptGame::initialize(){
	lua_getglobal(lua_state, "Initialize");
	if (lua_pcall(lua_state, 0, 0, 0) != 0)
		return -1;
	return 0;
}
int ScriptGame::cleanup(){
	// Engine managed cleanup
	return 0;
}