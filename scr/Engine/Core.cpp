#include "Core.h"

namespace Pro{

	Core::Core(){ 
		running = false; 
		window = new Window();
	}

	void Core::run(){ 
		timer->startUpdate();
		update(); 
		timer->endUpdate();

		timer->startRender();
		render();
		timer->endRender();
		
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

	void Core::addEntity(GameObject::Entity* entity){
		scene->addEntity(entity);
	}

	void Pro::Core::removeEntity(GameObject::Entity entity){
		uint32 id = entity.getGUID();
		GUIDLookup::releaseGUID(id);
		scene->destoryEntity(id);
	}
	

	GameObject::Entity* Pro::Core::getEntity(uint32 _id){
		return scene->getEntity(_id);
	} 
	 
	// Network Functions

	Networking::TCPConnection* Pro::Core::netStartServer(){
		return network->startServer();
	}
	Networking::TCPConnection* Pro::Core::netConnectToServer(const std::string &IP){
		return network->connectToServer(IP);
	}

	// returns the amount of bytes recieved
	unsigned int Pro::Core::netRecv(Networking::TCPConnection* _con, void *buffer){
		return network->recv(_con, buffer);
	}
	// returns the amount of bytes recieved without clearing the buffer
	unsigned int Pro::Core::netPeek(Networking::TCPConnection* _con){
		return network->peek(_con);
	}
	// sends the buffer through the connection
	void Pro::Core::netSend(Networking::TCPConnection* connection, void* buffer, unsigned int bufferSize){
		network->send(connection, buffer, bufferSize);
	}
	/* sends the buffer through the connection and deletes the buffer  */
	void Pro::Core::netSendd(Networking::TCPConnection* connection, void* buffer, unsigned int bufferSize){
		network->sendd(connection, buffer, bufferSize);
	}

	// Resource Functions

	Asset::AnimatedSprite*	Pro::Core::getSpriteAnimations(const std::string& name){
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

	Scene::TileType* Pro::Core::getMapTile(unsigned int x, unsigned int y){
		return scene->getMap()->getTile(x, y);
	}
	std::vector<GameObject::Entity*> Pro::Core::pollMapTile(unsigned int x, unsigned int y){
		return scene->pollTile(Math::Vector2(static_cast<int>(x), static_cast<int>(y)));
	}
	std::vector<GameObject::Entity*> Pro::Core::pollMapTile(SDL_Rect pos){
		return scene->pollTile(Math::Vector2(pos.x, pos.y));
	}
	void Pro::Core::setActiveCamera(const std::string& name){
		scene->setActiveCamera(GUIDLookup::getGUID(name));
	}
	Scene::Camera* Pro::Core::getActiveCamera(){
		return scene->getActiveCamera();
	}
	Scene::Camera* Pro::Core::getCamera(const std::string& name){
		return scene->getCamera(GUIDLookup::getGUID(name));
	}
	void Core::addCamera(Scene::Camera* _cam){ 
		scene->addCamera(_cam);
	} 

	void Core::setWindowDimensions(SDL_Rect rect){
		window->setRect(rect);
	} 
	void Core::setWindowTitle(const std::string& name){
		window->setTitle(name); 
	}
	  
	int Core::lUpdate(lua_State* L){
		Core* p = *(static_cast<Core**>(lua_touserdata(L, 1)));
		p->run();
		return 0;
	}
	bool Core::start(){
		scene = new Scene::Scene();
		network = new Networking::Network();
		timer = new Timer();
		sprite_manager = new Graphics::SpriteManager();
		event_handeler = new EventHandeler();
		renderer = new Graphics::Renderer(sprite_manager, scene);

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
}