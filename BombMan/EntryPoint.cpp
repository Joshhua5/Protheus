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
	/*
	while (state != GameType::Exit && game.isRunning() == true){  
		SDL_RenderClear(sdl.renderer);
		// Event Queue 
		SDL_Event event;
		if (SDL_PollEvent(&event) == 1)
			if (event.quit.type == SDL_QUIT)
				break;  

		switch (state){
			case GameType::MULTI_SERVER_WAIT:
				menu.render(sdl.renderer); 
				// start server
				//game.setup(true, "", sdl.renderer);
				state = GameType::MultiPlayer;
				break;
			case GameType::MULTI_CLIENT_WAIT:
				menu.render(sdl.renderer);
				// connect to server
				//game.setup(false, menu.getAddress(), sdl.renderer);
				state = GameType::MultiPlayer;
				break;

			case GameType::Menu: 
				if (menu.hasInitialized() == false)
					menu.setup(sdl.renderer, sdl.window); 
				state = menu.update(); 
				menu.render(sdl.renderer);
			break;
			case GameType::MultiPlayer:  
				game.update();
				//game.render(sdl.renderer);
				break;
			case GameType::SinglePlayer:
				state = GameType::Exit;
				break;
		}   
		SDL_RenderPresent(sdl.renderer);
		*/ 
	//menu.cleanup();   
} 

