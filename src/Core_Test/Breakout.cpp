#include <Graphics\Window.h>
#include <Graphics\SpriteBatcher.h> 
#include <Timer.h>
#include <FileSystem\FileSystem.h>
#include <Graphics\TextureLoader.h>
#include <ArrayList.h>
#include <Audio\Audio.h>
#include <Input\Keyboard.h>
#include <ProMath.h>
#include <iostream>


using namespace Pro;
using namespace Graphics;
using namespace Util;
using namespace Input;
using namespace Audio;

struct Entity {
	Vector2<float> position;
	Vector2<float> dimensions;
	Vector2<float> velocity;

	Entity(){}

	inline void update() {
		position += velocity;
	}
};

int main() {
	// Load Window
	auto window = Window("Breakout", { 1600, 800 });

	FileSystem textures("textures/");
	FileSystem audiofs("Audio/");

	Timer<> timer;
	Keyboard keyboard = window.keyboard();
	CAudio audio;
	CAudioListener listener;
	listener.setActive();
	listener.setGain(1);
	listener.position(Vector3<float>(.5f, 0, 0));
	SpriteBatcher batcher(window.dimensions().Cast<float>());

	// Load Textures

	auto paddle = TextureLoader::LoadTexture(textures.GetFile("Paddle.bmp"));
	auto ball = TextureLoader::LoadTexture(textures.GetFile("Ball.bmp"));

	auto ball_id = batcher.AttachTexture(ball);
	auto paddle_id = batcher.AttachTexture(paddle);
	 
	// Load Audio
	 
	auto boing_data = audio.loadAudio(AUDIO_FORMAT::WAV, &audiofs.GetFile("boing.wav"));
	auto boing = audio.createSource(boing_data);
	 
	// Initialize Entities

	Entity e_paddle;
	e_paddle.position = { window.width() / 2.f,  64.f };
	e_paddle.dimensions = paddle->dimensions().Cast<float>();

	Entity e_ball;
	e_ball.position = window.dimensions().Cast<float>() / 2.f;
	e_ball.dimensions = ball->dimensions().Cast<float>();
	e_ball.velocity = { 3, 3 };
	 
	// Create blocks

	ArrayList<Entity> blocks(50);
	 
	Entity e_block;
	e_block.dimensions = { 30, 15 };
	for (unsigned x = 0; x < window.width() / (e_block.dimensions.x + 15); ++x)
		for (unsigned y = 0; y < 5; ++y) {
			e_block.position = { x * (e_block.dimensions.x + 15), window.height() - (y * (e_block.dimensions.y + 15)) };
			e_block.velocity = { 0, 0 };
			blocks.PushBack(e_block);
		}

	// Game Loop
	while (!window.IsExitRequested()) {
		window.StartFrame();
		timer.Tick(); 
		// Input

		if ((bool)keyboard.IsKeyDown(KEY::KEY_A) && e_paddle.position.x > 0)
			e_paddle.position.x -= 5;
		if ((bool)keyboard.IsKeyDown(KEY::KEY_D) && e_paddle.position.x + e_paddle.dimensions.x <= window.width())
			e_paddle.position.x += 5;

		// Update

		e_ball.update();

		// Check ball with window 
		if (e_ball.position.x + e_ball.dimensions.x >= window.width() || e_ball.position.x <= 0)
			e_ball.velocity.x *= -1;
		if (e_ball.position.y + e_ball.dimensions.y >= window.height() || e_ball.position.y <= 0)
			e_ball.velocity.y *= -1; 

		// Check ball with paddle
		 
		if (e_ball.position.y < e_paddle.position.y + e_paddle.dimensions.y &&
			e_ball.position.x <= e_paddle.position.x + e_paddle.dimensions.x &&
			e_ball.position.x + e_ball.dimensions.x >= e_paddle.position.x) {
			e_ball.velocity.y *= -1; 
			boing.position(Vector3<float>(e_ball.position.x / window.width(), 0, 0));  
			boing.play();
		}

		// Check ball with blocks

		for (unsigned x = 0; x < blocks.size(); ++x)
		if (e_ball.position.y < blocks[x].position.y + blocks[x].dimensions.y &&
			e_ball.position.y + e_ball.dimensions.y >= blocks[x].position.y &&
			e_ball.position.x <= blocks[x].position.x + blocks[x].dimensions.x &&
			e_ball.position.x + e_ball.dimensions.x >= blocks[x].position.x) {
			e_ball.velocity.y *= -1;
			boing.position(Vector3<float>(e_ball.position.x / window.width(), 0, 0));
			boing.play();
			blocks.Erase({ x });
		}
		 
		// Render

		for (unsigned x = 0; x < blocks.size(); ++x)
			batcher.Push(paddle_id, ToVector3<float>(blocks.At(x).position), blocks.At(x).dimensions);

		batcher.Push(paddle_id, ToVector3<float>(e_paddle.position), e_paddle.dimensions);
		batcher.Push(ball_id, ToVector3<float>(e_ball.position), e_ball.dimensions);
		  
		batcher.Flush();
		window.EndFrame();
	}
}