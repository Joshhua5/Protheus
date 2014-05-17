#include "Main.h"
 

int main(int argc, char* args[])
{
	Pro::Core* core = new Pro::Core();
	Pro::Lua::Lua* lua = new Pro::Lua::Lua();
	lua->attachCore(core);
	lua->loadConfig("script\\Config.lua");
	SDL_Init(SDL_INIT_EVERYTHING);
	
	core->start();

	// all move into lua function lStartEngine
	 
	lua->loadResources();
	lua->loadMain();

	// Load Camera 
	Pro::Scene::Camera* camera = new Pro::Scene::Camera("camera1");
	camera->setDimensions(Math::Vector2(18, 18));
	camera->setPosition(Math::Vector2(0, 0));
	core->addCamera(camera);
	core->setActiveCamera("camera1");
	 
	while (!SDL_QuitRequested())
		core->run(); 

	core->cleanup();
	delete core;
	return 0;
}

