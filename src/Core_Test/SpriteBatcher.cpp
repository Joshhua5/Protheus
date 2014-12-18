#include <Graphics\SpriteBatcher.h>
#include <Graphics\Window.h>
#include <Graphics\TextureLoader.h>
#include <FileSystem\FileSystem.h>

using namespace Pro;



int main() {
	Graphics::Window window("Sprite Test", { 800, 800 });

	Util::FileSystem fs;

	smart_pointer<Texture> tex1 = TextureLoader::loadTexture(fs.getFile("textures/box.bmp"));
	smart_pointer<Texture> tex2 = TextureLoader::loadTexture(fs.getFile("textures/text.bmp"));
	smart_pointer<Texture> tex3 = TextureLoader::loadTexture(fs.getFile("textures/ban.bmp"));

	Graphics::SpriteBatcher batcher;

	auto batch1 = batcher.attachTexture(tex1);
	auto batch2 = batcher.attachTexture(tex2);
	auto batch3 = batcher.attachTexture(tex3);

	batcher.push(batch1, { 0.0f, 0.0f, 0.0f }, { 1, 1 });
	batcher.push(batch2, { -1.0f, -1.0f, 0.0f }, { 0.5, 0.5 });
	batcher.push(batch3, { 0.0f, -1.0f, 0.0f }, { 0.1f, 0.1f });

	while (true) {
		window.startFrame();

		batcher.render();

		window.endFrame();
	}
}