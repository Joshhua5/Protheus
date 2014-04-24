#include "Main.h"
 

int main(int argc, char* args[])
{
	Pro::Core* core = new Pro::Core();

	SDL_Rect dim;
	dim.x = 200;
	dim.y = 200;
	dim.w = 800;
	dim.h = 600;
	core->setWindowDimensions(dim);
	core->setWindowTitle("Core");

	core->start();
	 
	if (core->loadSpriteSheet(".\\textures\\SpriteSheet.PNG", "textures\\SpriteSheet.txt") == false)
		std::cout << "Error Loading Sprite Sheet";
	if (core->loadSpriteAnimations(".\\textures\\animations.txt") == false)
		std::cout << "Error Loading Sprite Animations";
	if (core->loadScene(".\\levels\\demoLevel.txt", ".\\levels\\demoLevelData.txt") == false)
		std::cout << "Error Loading Scene";

	// Load Camera 
	Pro::Camera camera;
	camera.setDimensions(0, 0, 18, 18);
	core->addCamera(camera, "camera1");
	core->setActiveCamera("camera1");
	 
	 
	while (!SDL_QuitRequested())
	{
		core->run(); 
	}
	core->cleanup();
	delete core;
	return 0; 
}

