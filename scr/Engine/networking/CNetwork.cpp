#include "CNetwork.h" 
namespace Pro{
	namespace Networking{

		Network::Network()
		{ 
		}


		Network::~Network()
		{ 
		}
		 

		void Network::serverUpdate(ServerTCPConnection* connection){
			
		}

		bool Network::init(){
			if (SDLNet_Init() < 0){
				SDL_LogError(SDL_LOG_CATEGORY_CUSTOM, "Network Init Failure");
				return false;
			}
			return true;
		}

		ServerTCPConnection* Network::startServer(){
			auto _server = new ServerTCPConnection(); 
			// Creates an Address for the socket's creation
			if (SDLNet_ResolveHost(_server->serverAddress, NULL, 9910)){
				SDL_LogError(SDL_LOG_CATEGORY_CUSTOM, "Network Listen Failure");
				delete _server;
				return nullptr;
			}
			// Opens the socket
			if (!(_server->serverSock = SDLNet_TCP_Open(_server->serverAddress))){
				delete _server;
				return nullptr;
			}
			// Start listening for a port
			std::thread* thread = new std::thread(&Network::serverUpdate, this, *_server);
			thread->detach();
			return _server;
		}

		ClientTCPConnection* Network::connectToServer(const std::string &IP){
			// Create Connection
			ClientTCPConnection* _connection = new ClientTCPConnection;

			// Check the address for open socket to connect to
			if (SDLNet_ResolveHost(_connection->serverAddress, IP.c_str(), 9910)){
				SDL_LogError(SDL_LOG_CATEGORY_RESERVED1, "Network Listen Failure");
				return nullptr;
			}
			// start trying to connect to the server
			std::thread* thread = new std::thread(&Network::clientUpdate, this, *_connection);
			thread->detach();
			connections.push_back(_connection);
			return _connection;
		}


		void Network::closeAll(){ 
			for (unsigned int x = 0; x < connections.size(); x++){
				connections[x]->connected = false;
			}
		}

		void Network::cleanup(){ 
			// close all client connections
			while (!connections.empty())
				connections.erase(connections.begin());
			SDLNet_Quit();
		} 
	}
}