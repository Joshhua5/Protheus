#include <Graphics\Window.h>
#include <Graphics\SpriteBatcher.h> 
#include <Timer.h>
#include <FileSystem\FileSystem.h>
#include <Graphics\TextureLoader.h>
#include <ArrayList.h> 
#include <ProMath.h>
#include <DoubleBuffer.h>
#include <iostream>

unsigned const ball_count = 1E2;

using namespace Pro;
using namespace Graphics;
using namespace Util; 

struct args { 
	Vector2<float> window;
	SpriteBatcher* spt;
	unsigned texture_id;
};

struct Entity {
	Vector2<float> position;
	Vector2<float> dimensions;
	Vector2<float> velocity;

	Entity(){}


	inline void update(void* window_) {
		args* window = (args*)window_;
		if (position.x + dimensions.x >= window->window.x || position.x <= 0)
			velocity.x *= -1;
		if (position.y + dimensions.y >= window->window.y || position.y <= 0)
			velocity.y *= -1;

		position += velocity;
	}
};

int main() {
	// Load Window
	auto window = Window("Breakout", { 1920, 1080 });

	FileSystem textures("textures/");

	Timer timer;
	srand(timer.tick());
	Keyboard keyboard = window.getKeyboard();

	SpriteBatcher batcher(window.getDimensions().cast<float>());

	// Load Textures
	 
	auto ball = TextureLoader::loadTexture(textures.getFile("Ball.bmp")); 
	auto ball_id = batcher.attachTexture(ball); 
	  
	// Initialize Entities
	 
	Entity e_ball; 
	e_ball.dimensions = ball->getDimensions().cast<float>(); 

	ArrayList<Entity> entities;

	for (unsigned x = 0; x < ball_count; ++x) {
		e_ball.position = Vector2<float>(prand<float>(0, window.getWidth()), prand<float>(0, window.getHeight()));
		e_ball.velocity = Vector2<float>(prand<float>(1, 5), prand<float>(1, 5));
		entities.push_back(e_ball);
	}

	batcher.alpha(Vector3<float>(1.f, 1.f, 1.f));

	// Game Loop
	while (!window.isExitRequested()) {
		window.startFrame(); 

		Future finished; 
		args wnd;
		wnd.window = window.getDimensions().cast<float>();
		wnd.texture_id = ball_id;
		wnd.spt = &batcher;

		// Parallel isn't working currently
		//Parallel::process<Entity>(entities.data(), &Entity::update, ball_count, 0, &finished, &wnd);
		 

		for (unsigned x = 0; x < ball_count; ++x){ 

			if (entities[x].position.x + entities[x].dimensions.x >=  window.getDimensions().x || entities[x].position.x <= 0)
				entities[x].velocity.x *= -1;
			if (entities[x].position.y + entities[x].dimensions.y >= window.getDimensions().x || entities[x].position.y <= 0)
				entities[x].velocity.y *= -1;

			entities[x].position += entities[x].velocity;
			batcher.push(ball_id, toVector3<float>(entities[x].position), entities[x].dimensions);

		}
		batcher.flush();
		window.endFrame();
	}
}