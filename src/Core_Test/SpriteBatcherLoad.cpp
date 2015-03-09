#include <Graphics\Window.h>
#include <Graphics\SpriteBatcher.h> 
#include <Timer.h>
#include <FileSystem\FileSystem.h>
#include <Graphics\TextureLoader.h>
#include <ArrayList.h> 
#include <ProMath.h>
#include <DoubleBuffer.h>
#include <iostream>

unsigned const ball_count = static_cast<unsigned>(5E4);

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

	Timer<std::chrono::nanoseconds> timer;  
	Keyboard keyboard = window.keyboard();

	SpriteBatcher batcher(window.dimensions().Cast<float>());

	// Load Textures
	 
	auto ball = TextureLoader::LoadTexture(textures.GetFile("box32bit.bmp")); 
	auto ball_id = batcher.AttachTexture(ball); 
	  
	// Initialize Entities
	 
	Entity e_ball; 
	e_ball.dimensions = ball->dimensions().Cast<float>(); 

	ArrayList<Entity> entities;

	for (unsigned x = 0; x < ball_count; ++x) {
		e_ball.position = Vector2<float>(Rand<float>(0, window.width()), Rand<float>(0, window.height()));
		e_ball.velocity = Vector2<float>(Rand<float>(1, 5), Rand<float>(1, 5));
		entities.PushBack(e_ball);
	}

	batcher.Alpha(Vector3<float>(1.f, 1.f, 1.f));

	// Game Loop
	while (!window.IsExitRequested()) {
		window.StartFrame(); 

		Future finished; 
		args wnd;
		wnd.window = window.dimensions().Cast<float>();
		wnd.texture_id = ball_id;
		wnd.spt = &batcher;

		// Parallel isn't working currently
		//Parallel::process<Entity>(entities.data(), &Entity::update, ball_count, 0, &finished, &wnd);
		 

		for (unsigned x = 0; x < ball_count; ++x){ 

			if (entities[x].position.x + entities[x].dimensions.x >=  window.dimensions().x || entities[x].position.x <= 0)
				entities[x].velocity.x *= -1;
			if (entities[x].position.y + entities[x].dimensions.y >= window.dimensions().x || entities[x].position.y <= 0)
				entities[x].velocity.y *= -1;

			entities[x].position += entities[x].velocity;
			batcher.Push(ball_id, ToVector3<float>(entities[x].position), entities[x].dimensions);

		}
		batcher.Flush();
		window.EndFrame();
	}
}