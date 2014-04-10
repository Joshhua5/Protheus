#include "Game.h" 
#include "Bomb.h"

Game::Game(){
	connection = nullptr;
}
Game::~Game(){

}
   

void Game::setup(){

	SDL_Rect dim;
	dim.x = 200;
	dim.y = 200;
	dim.w = 800;
	dim.h = 600;
	core.setWindowDimensions(dim);
	core.setWindowTitle("Core"); 

	core.start();

	if (core.loadSpriteSheet(".\\textures\\SpriteSheet.PNG", "textures\\SpriteSheet.txt") == false)
		std::cout << "Error Loading Sprite Sheet";
	if (core.loadSpriteAnimations(".\\textures\\animations.txt") == false)
		std::cout << "Error Loading Sprite Animations";
	if (core.loadScene(".\\levels\\demoLevel.txt", ".\\levels\\demoLevelData.txt") == false)
		std::cout << "Error Loading Scene";

	// Load Camera

	Pro::Camera camera;
	camera.setDimensions(0, 0, 18, 18);
	core.addCamera(camera, "camera1");
	core.setActiveCamera("camera1");

	// Load players

	Pro::Avatar* newAvatar = new Pro::Avatar;
	newAvatar->setPosX(2);
	newAvatar->setPosY(2); 
	newAvatar->addAnimation(0, core.getSpriteAnimations("player"));
	newAvatar->activateAnim(0);
	core.addEntity(*newAvatar, "player1");

	Pro::Avatar* newAvatar2 = new Pro::Avatar;
	newAvatar2->setPosX(4);
	newAvatar2->setPosY(4);
	newAvatar2->addAnimation(0, core.getSpriteAnimations("player"));
	newAvatar2->activateAnim(0);
	core.addEntity(*newAvatar2, "player2");

	Pro::Item* bomb = new Pro::Item(); 
	core.addEntity(*bomb, "bombItem");

	// Share player positions // Network 
	connection = core.netStartServer();

	if (connection->isServer){ 
		core.netSendd(connection, core.getEntity("player1")->getPos(), sizeof(int)* 2);
		core.netSendd(connection, core.getEntity("player2")->getPos(), sizeof(int)* 2);
	} else {
		int dataRecieved = 0;

		while (dataRecieved <= 4 * sizeof(int)){
			dataRecieved = core.netPeek(connection);
		}
		// player positions have arrived
		int* buffer = new int[4];
		core.netRecv(connection, buffer);
		newAvatar->setPos(buffer[0], buffer[1]); 
		newAvatar2->setPos(buffer[2], buffer[3]);
		//core.getEntity("player2")->setPos(buffer[2], buffer[3]); 
		delete[] buffer;
	}
} 

void Game::keyboardEvents(){
	SDL_Event event; 
	char* buffer = new char[sizeof(SDL_SCANCODE_LEFT)];
	unsigned int bufferSize = sizeof(SDL_SCANCODE_LEFT);
	SDL_Rect* pos;
	std::vector<SDL_Event>* keyboardEvents = core.pollKeyEvents();

	while (!keyboardEvents->empty()){
		event = keyboardEvents->back();
		keyboardEvents->pop_back();
		if (event.key.repeat == 0 && event.key.type == SDL_KEYDOWN){
			switch (event.key.keysym.scancode){
			case SDL_SCANCODE_LEFT:
				pos = core.getEntity("player1")->getPositionRectP();
				if (core.getMapTile(pos->x - 1, pos->y)->passable){
					pos->x -= 1;
					*buffer = SDL_SCANCODE_LEFT;
					core.netSend(connection, buffer, bufferSize);
				}
			case SDL_SCANCODE_RIGHT:
				pos = core.getEntity("player1")->getPositionRectP();
				if (core.getMapTile(pos->x + 1, pos->y)->passable){
					pos->x += 1;
					*buffer = SDL_SCANCODE_RIGHT;
					core.netSend(connection, buffer, bufferSize);
				}
				break;

			case SDL_SCANCODE_UP:
				pos = core.getEntity("player1")->getPositionRectP();
				if (core.getMapTile(pos->x, pos->y - 1)->passable){
					pos->y -= 1;
					*buffer = SDL_SCANCODE_UP;
					core.netSend(connection, buffer, bufferSize);
				}
				break;
			case SDL_SCANCODE_DOWN:
				pos = core.getEntity("player1")->getPositionRectP();
				if (core.getMapTile(pos->x, pos->y + 1)->passable){
					pos->y += 1;
					*buffer = SDL_SCANCODE_DOWN;
					core.netSend(connection, buffer, bufferSize);
				}
				break;
			case SDL_SCANCODE_SPACE:
				// Code for player dropping bomb here

				/* old code
				if (players[0].hasBomb){
				loadedLevel->bombs.push_back(players[0].bomb);
				players[0].bomb->activated = true;
				players[0].bomb->timer_started = SDL_GetTicks();
				players[0].hasBomb = false;
				players[0].bomb[0].position[0] = players[0].getPosition()[0];
				players[0].bomb[0].position[1] = players[0].getPosition()[1];
				players[0].bomb = nullptr;
				network.send((int) networkCommand::DROP_BOMB, isServer);
				*/
				break;
			}
		}
	}
}

void Game::update(){
	// System Events 
	keyboardEvents();

	// Get engine working first


	// 
	//Pro::Avatar* player1 = (Pro::Avatar*)core.getEntity("player1"); 
	//Pro::Item* bombItem = (Pro::Item*)core.getEntity("bombItem");
	//if (player1 == nullptr)
	//	return;

	//// see if player is on top of a bomb
	//std::vector<Pro::Entity*> pollResult = core.pollMapTile(player1->getPositionRect());
	//for each(Pro::Entity* obj in pollResult){
	//	// Check if a bomb
	//	if (!(bombItem = dynamic_cast<Bomb*>(obj)))
	//		continue;
	//	// Check player inventory if they already have a bomb
	//	if (!player1->checkForItem(bombItem)){
	//		if (bomb->activated == false){
	//			// give bomb to player
	//			player1->giveItem(bombItem); 
	//			core.removeEntity(*bomb);
	//			//	 Send network command
	//		}
	//	}
	//}

	//// Update activated bombs
	//unsigned int ticks = SDL_GetTicks();
	//for each(auto obj in *core.getEntities()){
	//	if (!(bomb = dynamic_cast<Bomb*>(&obj.second)))
	//		continue;
	//	if (bomb->activated)
	//		bomb->countDown = (unsigned short)ceil((ticks - bomb->timer_started) / 1000);

	//	if (bomb->countDown == 2){
	//		// explode
	//		//std::vector<int*> activeTiles = loadedLevel->explosionAt(players, bomb->position, &running);
	//		// check if player is in the explosion area

	//		// start the animations 
	//		// cleanup 
	//		//loadedLevel->bombs.erase(loadedLevel->bombs.begin() + x);
	//		break;
	//	}
	//	networkUpdate();
	//}
}

void Game::render(){ 
	core.run();
}

void Game::networkUpdate(){
	//  Network recieve code 
	SDL_Scancode* buffer = nullptr; 
	unsigned int size = core.netRecv(connection, buffer);
	if (size == 0 )
		return; 

	Bomb newBomb;
	Pro::Avatar* player2 = (Pro::Avatar*)core.getEntity("player2");
	for (unsigned int x = 0; x < size; x++){
		switch (buffer[x]){
		case SDL_SCANCODE_DOWN: 
			player2->getPositionRectP()->y++; 
			break;
		case SDL_SCANCODE_UP:
			player2->getPositionRectP()->y--;
			break;
		case SDL_SCANCODE_LEFT:
			player2->getPositionRectP()->x--;
			break;
		case SDL_SCANCODE_RIGHT:
			player2->getPositionRectP()->x++;
			break;
		case SDL_SCANCODE_F:
			// Picked up bomb 
			break;
		case SDL_SCANCODE_SPACE:
			newBomb.activated = true;
			newBomb.timer_started = SDL_GetTicks(); 
			player2->takeItem((Pro::Item*)core.getEntity("bombItem")); 
			newBomb.setPosX(player2->getPosX());
			newBomb.setPosY(player2->getPosY()); 
			break;
		} 
	} 
}


void Game::cleanup(){
	core.cleanup();
}
