#include "Main.h"
 
using namespace Pro;
using namespace Lua;

int main(int argc, char* args[])
{
	SDL_Init(SDL_INIT_EVERYTHING);
	CLua* lua = new CLua();
	IGame* game = nullptr;
	game = lua->loadConfig("config.lua"); 

	// execute if engine is ScriptDriven
	if (lua->isGameScriptMode()){ 
		lua->loadMain();
		lua->setGlobals(*static_cast<ScriptGame*>(game), lua->getLuaState());

		static_cast<ScriptGame*>(game)->initialize();
		static_cast<ScriptGame*>(game)->gameLoop();
		//static_cast<ScriptGame*>(game)->cleanup();
	}
	else{
		static_cast<DataGame*>(game)->initialize();
		static_cast<DataGame*>(game)->gameLoop();
		//static_cast<DataGame*>(game)->cleanup();
	}


	delete game; 
	delete lua;

	return 0;
}