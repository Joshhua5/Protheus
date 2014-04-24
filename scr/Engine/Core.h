#pragma once
#include "Scene.h"
#include "Renderer.h"
#include "Timer.h" 
#include "Network.h"
#include "Window.h"
#include "SpriteManager.h"
#include "Lua.h"
#include "EventHandeler.h"

namespace Pro{

	class Core
	{
		Scene* scene;
		Networking::Network* network;
		Graphics::Renderer* renderer;
		Window* window;
		Timer* timer;
		Lua::Lua* lua;
		Graphics::SpriteManager* sprite_manager;
		EventHandeler* event_handeler;
		IDManager* id_manager;

		bool running = false;

		bool update();
		bool render();
	public:

		// Core functions 
		Core::Core();
		  
		bool loadSpriteSheet(const std::string &path, const std::string &data);
		bool loadSpriteAnimations(const std::string& path);
		bool loadScene(const std::string &path, const std::string& data);

		// x : position X, y : position Y
		// w : width, h : height
		void setWindowDimensions(SDL_Rect rect);
		// Sets the name of the window
		void setWindowTitle(const std::string& name);
		/* Runs the loop once and stops,
		   for user modification of the map */
		void run();

		// Change window details before Starting the engine
		bool start();
		bool cleanup();

		// Game Functions

		void addEntity(Entity entity, const std::string&  name);
		void removeEntity(Entity entity); 
		std::unordered_map<ID, Entity>* getEntities();
		Entity* getEntity(ID _id);
		Entity* getEntity(const std::string&  name);
		ID getID(const std::string&  name);

		// Network Functions

		Networking::TCPConnection* netStartServer();
		Networking::TCPConnection* netConnectToServer(const std::string &IP);

		// returns the amount of bytes recieved
		unsigned int netRecv(Networking::TCPConnection*, void *buffer);
		// returns the amount of bytes recieved without clearing the buffer
		unsigned int netPeek(Networking::TCPConnection*);
		// sends the buffer through the connection
		void netSend(Networking::TCPConnection* connection, void* buffer, unsigned int bufferSize);
		/* sends the buffer through the connection and deletes the buffer  */
		void netSendd(Networking::TCPConnection* connection, void* buffer, unsigned int bufferSize);

		// Resource Functions
		 
		AnimatedSprite*	getSpriteAnimations(const std::string& name);

		// Event Functions

		std::vector<SDL_Event>* pollKeyEvents();
		std::vector<SDL_Event>* pollMouseEvents();
		std::vector<SDL_Event>* pollSystemEvents();
		std::vector<SDL_Event>* pollUserEvents();

		// Map Functions

		TileType* getMapTile(unsigned int x, unsigned int y);
		std::vector<Entity*> pollMapTile(unsigned int x, unsigned int y);
		std::vector<Entity*> pollMapTile(SDL_Rect pos);
		void setActiveCamera(const std::string& name);
		Camera* getActiveCamera();
		Camera* getCamera(const std::string& name);
		void addCamera(Camera, const std::string& name);
	}; 
}