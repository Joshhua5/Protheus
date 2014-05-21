#include "Main.h"
#include "Math.h"

using namespace Pro;

int main(int argc, char* args[])
{ 
	Lua::CLua* lua = new Lua::CLua(); 
	lua->loadConfig("script\\Config.lua");
	SDL_Init(SDL_INIT_EVERYTHING);
	
	//core->start();

	// all move into lua function lStartEngine
	 
	lua->loadResources();
	lua->loadMain();


	//// Load Camera Debugging (only while there's no demo game to load
	//Scene::Camera* camera = new Scene::Camera("camera1");
	//camera->setDimensions(Math::Vector2(18, 18));
	//camera->setPosition(Math::Vector2(0, 0));
	//

	//Util::luaP_registerget<Scene::Scene>(lua->getLuaState, "ACTIVE_SCENE")->addCamera(camera);
	//    
	//while (!SDL_QuitRequested())
	//	core->run(); 

	//core->cleanup();
	delete lua;
	return 0;
}

