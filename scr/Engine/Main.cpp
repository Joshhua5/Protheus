
#include "Main.h" 

using namespace Pro;

int main(int argc, char* args[])
{ 
	SDL_Init(SDL_INIT_EVERYTHING);
	Lua::CLua* lua = new Lua::CLua();
	IGame* game = nullptr;
	game = lua->loadConfig("..\\GameDemo\\Config.lua");
	 
	ScriptGame* sGame = dynamic_cast<ScriptGame*>(game);
	DataGame* dGame = dynamic_cast<DataGame*>(game);

	if (sGame != nullptr){
		lua->loadResources();
		lua->loadMain();

		sGame->initialize(); 
		sGame->gameLoop();
		sGame->cleanup();
	}
	 
	if (dGame != nullptr){
		dGame->initialize();
		dGame->gameLoop();
		dGame->cleanup();
	}

	delete dGame;
	delete lua;
	 
	return 0;
}