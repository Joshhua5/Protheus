#pragma once  
#include "Engine\Core.h"
#include "Engine\Avatar.h"
#include <iostream>
#include <vector>

class Game
{
private:  
	Pro::Core core; 
	 
	void systemEvents(); 

	void loadResources(SDL_Renderer* sdl);
	void networkUpdate();
	void keyboardEvents();

	// have the GUI initialize the connection
	Pro::TCPConnection* connection;

	
public: 
	Game();
	~Game();

	void setup();
	void update();
	void render();
	void cleanup();
};

