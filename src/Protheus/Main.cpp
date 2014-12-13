//#include "Main.h" 

#include <Graphics\Window.h>
#include <Graphics\Sprite.h>
#include <Graphics\TextureLoader.h>
#include <Audio\WavDecoder.h>
#include <Audio\Audio.h>
#include <FileSystem\FileSystem.h>
#include <thread>
#include <Vector4.h> 

using namespace Pro; 
using namespace Util;
using namespace Audio; 

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
	auto audiof = fileIO.getFile("test.wav");

	CAudio audio; 
	auto buffer = audio.loadAudio(AUDIO_FORMAT::WAV, &audiof); 
	auto source = audio.createSource(buffer); 
	source.play(); 

	auto tex = TextureLoader::loadTexture(&file);

	//Graphics::Sprite sprite(tex);
	  

	while (true) { 
		window.startFrame();
		source.play();  
		window.endFrame();
	}
	//batch.push(&sprite, Vector4(0, 0, 0, 1)); 
	//batch.flush();
}