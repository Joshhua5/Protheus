#include <Graphics\Window.h>
#include <Graphics\SpriteBatcher.h>  
#include <FileSystem\FileSystem.h>
#include <Graphics\TextureLoader.h> 
#include <ProMath.h>   
#include "AnimatedSpriteBatcher.h"

using namespace Pro;
using namespace Graphics;
using namespace Util;

int main() {
	// Load Window
	auto window = Window("Breakout", { 128, 128 });

	FileSystem textures("textures/"); 

	// Use this class to instead of SpriteBatcher for animation support
	AnimatedSpriteBatcher batcher(window.dimensions().Cast<float>());

	// Load Animation
	
	smart_ptr<AnimatedSprite> a_sprite = new AnimatedSprite();
	// Add frames to animations
	a_sprite->addFrame(TextureLoader::LoadTexture(textures.GetFile("Ball.bmp")));
	a_sprite->addFrame(TextureLoader::LoadTexture(textures.GetFile("ban.bmp")));
	a_sprite->addFrame(TextureLoader::LoadTexture(textures.GetFile("box.bmp")));
	a_sprite->addFrame(TextureLoader::LoadTexture(textures.GetFile("text.bmp")));
	// Attach Animations to batcher
	auto a_id = batcher.AttachAnimation(a_sprite); 

	// Game Loop
	while (!window.IsExitRequested()) {
		window.StartFrame();
		 
		// Draw Animations
		batcher.Push(a_id, { 0, 0, 0 }, { (float)window.width(), (float)window.height() });
		// Render and Reset
		batcher.Flush();
		// Swap the animations to the next sprite
		batcher.IncrementAnimation(a_id); 

		// Note All sprites will be incremented of the a_id
		// To have two animations which are independent,
		// Create a copy of a_sprite and then attach that animations
		// and use it's returned id.
		 
		batcher.Flush();
		window.EndFrame();
	}
}