#include "Main.h"
 

int main(int argc, char* args[])
{
	Pro::Core* core = new Pro::Core();
	Pro::Lua::Lua* lua = new Pro::Lua::Lua();
	lua->attachCore(core);
	lua->loadConfig("script\\Config.lua");
	
	core->start();

	lua->loadResources();
	lua->loadMain();

	// Load Camera 
	Pro::Camera* camera = new Pro::Camera();
	camera->setDimensions(18, 18);
	camera->setPosition(0, 0);
	core->addCamera(camera, "camera1");
	core->setActiveCamera("camera1");
	 
	while (!SDL_QuitRequested())
		core->run(); 

	core->cleanup();
	delete core;
	return 0; 
}

