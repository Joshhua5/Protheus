#include <Graphics\SpriteBatcher.h>
#include <Graphics\Window.h>
#include <Graphics\TextureLoader.h>
#include <FileSystem\FileSystem.h>

using namespace Pro;

 

int main() {  
	Graphics::Window window("Sprite Test", { 800, 800 });

	Util::FileSystem fs;

	Texture* tex1 = TextureLoader::loadTexture(fs.getFile("bannana.bmp"));
	Texture* tex2 = TextureLoader::loadTexture(fs.getFile("text.bmp"));

	Graphics::SpriteBatcher batcher;

	auto batch1 = batcher.attachTexture(*tex1);
	auto batch2 = batcher.attachTexture(*tex2);

	while (true) {
		window.startFrame();

		batcher.push(batch1, { 0, 0, 0 }, { 1, 1 });
		batcher.push(batch2, { -1, -1, 0 }, { 0.5, 0.5 });

		batcher.flush();

		window.endFrame();
	} 
}