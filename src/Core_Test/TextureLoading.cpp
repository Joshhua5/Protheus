
#include <Graphics\Window.h>
#include <Graphics\Sprite.h>
#include <Graphics\SpriteBatcher.h>
#include <Graphics\TextureLoader.h>
#include <Audio\WavDecoder.h>
#include <Audio\Audio.h>
#include <FileSystem\FileSystem.h>
#include <thread>
#include <Vector4.h>  
#include <Log.h>

using namespace Pro;
using namespace Util;
using namespace Graphics;
using namespace Audio; 

int main(int argc, char* args []) {
	auto window = Graphics::Window("Window", Vector2<int>(128, 128));

	FileSystem fileIO;

	auto file = fileIO.GetFile("text.bmp"); 
	auto tex = Graphics::TextureLoader::LoadTexture(&file);
	
	if (tex.IsNull()) {
		global_log.Report<LogCode::ERROR>("Unable to load texture: text.bmp", __FUNCTION__, __LINE__); 
	}
	else {
		SpriteBatcher batch;
		batch.Push(batch.AttachTexture(tex), { 0, 0, 0 }, { 128, 128 });
		while (!window.IsExitRequested()) {
			window.StartFrame();
			batch.Render();
			window.EndFrame();
		}
	}
	// Clean up since there's no system in place to close some features

	// Smart pointer will delete the texture since it's the last reference
	tex = nullptr;
	global_log.Close();
}