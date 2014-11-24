//#include "Main.h"
#include <Graphics\Window.h>
#include <Graphics\Sprite.h>
#include <Graphics\Renderer.h>
#include <Vector4.h>
#include <SDL_image.h> 

using namespace Pro; 
using namespace Math;

//int main(int argc, char* args[])
//{
//	SDL_Init(SDL_INIT_EVERYTHING);
//	CLua* lua = new CLua();
//	IGame* game = nullptr;
//	game = lua->loadConfig("config.lua"); 
//
//	// execute if engine is ScriptDriven
//	if (lua->isGameScriptMode()){ 
//		lua->loadMain();
//		lua->setGlobals(*static_cast<ScriptGame*>(game), lua->getLuaState());
//
//		static_cast<ScriptGame*>(game)->initialize();
//		static_cast<ScriptGame*>(game)->gameLoop();
//		//static_cast<ScriptGame*>(game)->cleanup();
//	}
//	else{
//		static_cast<DataGame*>(game)->initialize();
//		static_cast<DataGame*>(game)->gameLoop();
//		//static_cast<DataGame*>(game)->cleanup();
//	}
//
//
//	delete game; 
//	delete lua;
//
//	return 0;
//}

// Layer 2 test
int main(int argc, char* args []) {
	auto window = Graphics::Window("Window");
	auto render = Graphics::Renderer(window);
	auto batch = Graphics::SpriteBatcher(render.getRenderer());

	IMG_Init(IMG_INIT_PNG); 
	Graphics::Sprite spt(IMG_LoadTexture(render.getRenderer(), "test.png"));
	
	render.startFrame();
	
	batch.push(&spt, Vector4(0, 0, 0, 1));

	batch.flush();

	render.endFrame();

	while (true);

}