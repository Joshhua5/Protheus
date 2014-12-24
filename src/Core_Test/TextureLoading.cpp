
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
using namespace Math;

int main(int argc, char* args []) {
	auto window = Graphics::Window("Window", Vector2<int>(800, 600));

	FileSystem fileIO;

	auto file = fileIO.getFile("text.bmp"); 
	auto tex = TextureLoader::loadTexture(&file);
	  
	// Smart pointer will delete the texture since it's the last reference
	tex = nullptr;
}