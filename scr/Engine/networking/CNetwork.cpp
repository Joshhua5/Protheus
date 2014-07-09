#include "CNetwork.h"
namespace Pro{
	namespace Networking{
		Network::Network()
		{
		}

		Network::~Network()
		{
		}

		bool Network::init(){
			if (SDLNet_Init() < 0){
				SDL_LogError(SDL_LOG_CATEGORY_CUSTOM, "Network Init Failure");
				return false;
			}
			return true;
		}

		ServerTCPConnection* Network::startServer(){
			// Create Connection
			ServerTCPConnection* _connection = new ServerTCPConnection;

			// Check the address for open socket to connect to
			if (SDLNet_ResolveHost(_connection->serverAddress, NULL, 9910)){
				SDL_LogError(SDL_LOG_CATEGORY_RESERVED1, "Network Listen Failure");
				return nullptr;
			}
			// start trying to connect to the server
			_connection->startHandshake();
			connections.push_back(_connection);
			return _connection;
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
			_connection->startHandshake();
			connections.push_back(_connection);
			return _connection;
		}

		void Network::closeAll(){
		}

		void Network::cleanup(){
			// close all client connections
			while (!connections.empty())
				connections.erase(connections.begin());
			SDLNet_Quit();
		}
	}
}