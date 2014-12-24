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
	auto window = Window("Breakout", { 800, 600 });

	FileSystem textures("textures/");
	FileSystem audio("Audio/");

	Timer timer;
	Keyboard keyboard = window.getKeyboard();

	SpriteBatcher batcher(window.getDimensions().cast<float>());

	// Load Textures

	auto paddle = TextureLoader::loadTexture(textures.getFile("Paddle.bmp"));
	auto ball = TextureLoader::loadTexture(textures.getFile("Ball.bmp"));

	auto ball_id = batcher.attachTexture(ball);
	auto paddle_id = batcher.attachTexture(paddle);
	 
	// Initialize Entities

	Entity e_paddle;
	e_paddle.position = { window.getWidth() / 2.f,  64.f };
	e_paddle.dimensions = paddle->getDimensions().cast<float>();

	Entity e_ball;
	e_ball.position = window.getDimensions() / 2.f;
	e_ball.dimensions = ball->getDimensions().cast<float>();
	e_ball.velocity = { 3, 3 };

	// Create panels

	ArrayList<Entity> blocks(50);

	// Game Loop
	while (!window.isExitRequested()) {
		window.startFrame();
		timer.tick();
		std::cout << timer.getTicksPerSec() << "\n";
		// Input

		if ((bool)keyboard.isKeyDown(KEY::KEY_A))
			e_paddle.position.x -= 5;
		if ((bool)keyboard.isKeyDown(KEY::KEY_D))
			e_paddle.position.x += 5;

		// Update

		e_ball.update();

		// Check ball with window 
		if (e_ball.position.x + e_ball.dimensions.x >= window.getWidth() || e_ball.position.x <= 0)
			e_ball.velocity.x *= -1;
		if (e_ball.position.y + e_ball.dimensions.y >= window.getHeight() || e_ball.position.y <= 0)
			e_ball.velocity.y *= -1; 

		// Check ball with paddle
		 
		if (e_ball.position.y < e_paddle.position.y + e_paddle.dimensions.y && 
			e_ball.position.x <= e_paddle.position.x + e_paddle.dimensions.x &&
			e_ball.position.x + e_ball.dimensions.x >= e_paddle.position.x)
			e_ball.velocity.y *= -1;

		// Render

		batcher.push(paddle_id, toVector3<float>(e_paddle.position), e_paddle.dimensions);
		batcher.push(ball_id, toVector3<float>(e_ball.position), e_ball.dimensions);

		 
		batcher.flush();
		window.endFrame();
	}
}