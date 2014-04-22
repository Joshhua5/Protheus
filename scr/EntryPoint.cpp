#include "Game.h"
 
int main(int argc, char* args [])
{ 
	Game* game = new Game();
	game->setup();
	while (!SDL_QuitRequested())
	{
		game->update();
		game->render();
	}
	game->cleanup();
	delete game;
	return 0;

} 

