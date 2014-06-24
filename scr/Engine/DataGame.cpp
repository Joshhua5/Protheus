#include "DataGame.h"

using namespace Pro;

DataGame::DataGame(lua_State* L)
{
	lua_state = L;
}

DataGame::DataGame()
{
	lua_state = nullptr;
}


DataGame::~DataGame()
{
}


int DataGame::update(){ 
	return 0;
}

int DataGame::render(){
	return 0;
}

int DataGame::initialize(){
	return 0;
}

int DataGame::cleanup(){
	return 0;
}

int DataGame::gameLoop(){
	return 0;
}