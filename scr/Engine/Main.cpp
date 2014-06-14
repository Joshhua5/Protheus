
#include "Main.h" 

using namespace Pro;

int main(int argc, char* args[])
{ 
	Lua::CLua* lua = new Lua::CLua();
	lua->loadConfig("..\\GameDemo\\Config.lua");
	SDL_Init(SDL_INIT_EVERYTHING);

	lua->loadResources();
	lua->loadMain();

	delete lua;
	return 0;
}