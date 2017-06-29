#include <Graphics/Window.h>
#include <Graphics/SpriteBatcher.h> 
#include <Timer.h>
#include <FileSystem/FileSystem.h>
#include <Graphics/TextureLoader.h>
#include <ArrayList.h> 
#include <ProMath.h>
#include <iostream>

unsigned const ball_count = static_cast<unsigned>(1E3);

using namespace Pro;
using namespace Graphics;
using namespace Util;
using namespace Input;
using namespace Math;

struct Entity {
	Vector2<float> position;
	Vector2<float> dimensions;
	Vector2<float> velocity;

	Entity(){}
	 
	inline void update(const Vector2<float>& window) {
		if (position.x + dimensions.x >= window.x || position.x <= 0)
			velocity.x *= -1;
		if (position.y + dimensions.y >= window.y || position.y <= 0)
			velocity.y *= -1;

		position += velocity;
	}
};

int main() {
	// Load Window
	Window window("Breakout", { 1280, 720 });

	FileSystem textures("textures/");

	Timer<std::chrono::nanoseconds> timer;  
	Keyboard keyboard = window.keyboard();
	if (window.IsOpen() == false){
		global_log.Close();
		return 1;
	}
	SpriteBatcher batcher(window.dimensions().Cast<float>());

	// Load Textures
	 
	auto ball = TextureLoader::LoadTexture(textures.GetFile("height.bmp")); 
	
	if (ball == nullptr)
		return 1;
	
	auto ball_id = batcher.AttachTexture(ball); 
	   
	// Initialize Entities
	 
	Entity e_ball; 
	e_ball.dimensions = ball->dimensions().Cast<float>(); 

	ArrayList<Entity> entities;

	for (unsigned x = 0; x < ball_count; ++x) {
		e_ball.position = Vector2<float>(Rand<float>(0, window.width()), Rand<float>(0, window.height()));
		e_ball.velocity = Vector2<float>(Rand<float>(1, 5), Rand<float>(1, 5));
		entities.push_back(e_ball);
	}

	batcher.Alpha(Vector3<float>(1.f, 1.f, 1.f));

	// Game Loop
	while (!window.IsExitRequested()) {
		window.StartFrame();
		auto window_dim = window.dimensions().Cast<float>();
		 
		for (unsigned x = 0; x < ball_count; ++x){ 
            entities[x].update(window_dim);
            batcher.Push(ball_id, ToVector3<float>(entities[x].position), entities[x].dimensions);
		}
		batcher.Flush();
		window.EndFrame();
	}
}