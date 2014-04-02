#include "Core.h"

namespace Pro{

	Core::Core(){ 
		running = false;
		id_manager = new IDManager(); 
		window = new Window();
	}

	void Core::run(){ 
		timer->startUpdate();
		update(); 
		timer->endUpdate();
		timer->startRender();

		timer->endRender();
		render();
		timer->tick(); 
	}

	bool Core::update(){
		event_handeler->update();
		return true;
	}   

	bool Core::loadSpriteSheet(const std::string &path, const std::string &data){
		return sprite_manager->loadSpriteSheet(renderer->getRenderer(), path, data);
	}

	bool Core::loadScene(const std::string &path, const std::string& data){
		return scene->loadSceneData(path, data);
	}

	bool Core::loadSpriteAnimations(const std::string& path){
		return sprite_manager->loadAnimations(renderer->getRenderer(), path);
	}

	bool Core::start(){
		scene = new Scene();
		network = new Network(); 
		timer = new Timer();
		sprite_manager = new SpriteManager();
		event_handeler = new EventHandeler();
		renderer = new Renderer(sprite_manager, scene);

		SDL_Init(SDL_INIT_EVERYTHING);
		if (window->createWindow() == false)
			return false; 
		if (renderer->init(window->getWindow()) == false)
			return false;
		if (network->init() == false)
			return false;
		
		running = true; 
		return true;
	}
	  
	bool Core::cleanup(){

		network->cleanup();
		SDL_Quit();
		return true;
	}

	bool Core::render(){
		SDL_RenderClear(renderer->getRenderer());
		renderer->renderScene();
		SDL_RenderPresent(renderer->getRenderer());
		return true;
	} 
	 
	/*
			GAME FUNCTIONS
	*/

	void Pro::Core::addEntity(Entity entity, const std::string&  name){
		ID id = id_manager->getID(name);
		entity.setID(id);
		scene->addEntity(entity, id);
	}

	void Pro::Core::removeEntity(Entity entity){
		ID id = entity.getID();
		id_manager->releaseID(id);
		scene->destoryEntity(id);
	}
	

	Entity* Pro::Core::getEntity(ID _id){
		return scene->getEntity(_id);
	}
	Entity* Pro::Core::getEntity(const std::string&  name){
		return scene->getEntity(id_manager->getIDFromName(name));
	}
	ID Pro::Core::getID(const std::string&  name){
		return id_manager->getIDFromName(name);
	}

	// Network Functions

	TCPConnection* Pro::Core::netStartServer(){
		return network->startServer();
	}
	TCPConnection* Pro::Core::netConnectToServer(const std::string &IP){
		return network->connectToServer(IP);
	}

	// returns the amount of bytes recieved
	unsigned int Pro::Core::netRecv(TCPConnection* _con, void *buffer){
		return network->recv(_con, buffer);
	}
	// returns the amount of bytes recieved without clearing the buffer
	unsigned int Pro::Core::netPeek(TCPConnection* _con){
		return network->peek(_con);
	}
	// sends the buffer through the connection
	void Pro::Core::netSend(TCPConnection* connection, void* buffer, unsigned int bufferSize){
		network->send(connection, buffer, bufferSize);
	}
	/* sends the buffer through the connection and deletes the buffer  */
	void Pro::Core::netSendd(TCPConnection* connection, void* buffer, unsigned int bufferSize){
		network->sendd(connection, buffer, bufferSize);
	}

	// Resource Functions

	AnimatedSprite*	Pro::Core::getSpriteAnimations(const std::string& name){
		return sprite_manager->getAnim(name);
	}

	// Event Functions

	std::vector<SDL_Event>* Pro::Core::pollKeyEvents(){
		return event_handeler->pollKeyEvents();
	}
	std::vector<SDL_Event>* Pro::Core::pollMouseEvents(){
		return event_handeler->pollMouseEvents();
	}
	std::vector<SDL_Event>* Pro::Core::pollSystemEvents(){
		return event_handeler->pollSystemEvents();
	}
	std::vector<SDL_Event>* Pro::Core::pollUserEvents(){
		return event_handeler->pollUserEvents();
	}

	// Map Functions

	TileType* Pro::Core::getMapTile(unsigned int x, unsigned int y){
		return scene->getMapData()->getTile(x, y);
	}
	std::vector<Entity*> Pro::Core::pollMapTile(unsigned int x, unsigned int y){
		return scene->pollTile(x, y);
	}
	std::vector<Entity*> Pro::Core::pollMapTile(SDL_Rect pos){
		return scene->pollTile(pos);
	}
	void Pro::Core::setActiveCamera(const std::string& name){
		scene->setActiveCamera(id_manager->getIDFromName(name));
	}
	Camera* Pro::Core::getActiveCamera(){
		return scene->getActiveCamera();
	}
	Camera* Pro::Core::getCamera(const std::string& name){
		return scene->getCamera(id_manager->getIDFromName(name));
	}
	void Pro::Core::addCamera(Camera _cam, const std::string& name){
		ID id = id_manager->getID(name);
		scene->addCamera(id, _cam);
	} 

	void Core::setWindowDimensions(SDL_Rect rect){
		window->setRect(rect);
	} 
	void Core::setWindowTitle(const std::string& name){
		window->setTitle(name); 
	}

	std::unordered_map<ID, Entity>* Core::getEntities(){
		return scene->getEntities();
	}
}