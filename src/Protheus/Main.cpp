//#include "Main.h" 

#include <Graphics\Window.h>
#include <Graphics\Sprite.h>
#include <Graphics\TextureLoader.h> 
#include <FileSystem\FileSystem.h>
#include <thread>
#include <Vector4.h> 

using namespace Pro; 
using namespace Util;
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
	auto window = Graphics::Window("Window", Vector2<int>(800, 600));  

	FileSystem fileIO;

	auto file = fileIO.getFile("text.bmp");

	Texture* tex = TextureLoader::loadTexture(&file);

	Graphics::Sprite sprite(tex); 
	 
	while (true) {
		std::this_thread::sleep_for(std::chrono::seconds(1));
		window.startFrame();
		window.endFrame();
	}
	//batch.push(&sprite, Vector4(0, 0, 0, 1)); 
	//batch.flush(); 
}