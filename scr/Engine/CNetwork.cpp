#include "CNetwork.h" 
namespace Pro{
	namespace Networking{

		Network::Network()
		{ 
		}


		Network::~Network()
		{ 
		}

		void Network::clientUpdate(ClientTCPConnection* connection){
			// wait for connection 
			while (!(connection->serverSock = SDLNet_TCP_Open(connection->serverAddress)))
				std::this_thread::sleep_for(std::chrono::milliseconds(10));
			  
			// Connected
			// Get the remote IP
			if ((connection->clientAddress = SDLNet_TCP_GetPeerAddress(connection->clientSock)))
				SDL_LogInfo(SDL_LOG_CATEGORY_RESERVED1, "Host Connected");
			else
				SDL_LogError(SDL_LOG_CATEGORY_RESERVED1, "Host Connection Failed");
			// use of mutex so we don't lock the connection threads
			connection->mutex.lock();
			connection->connected = true;
			connection->mutex.unlock();

			// Get data recieved 
			if (connection->connected){
				CBuffer inputBuffer(1024);
				inputBuffer.size = 0;
				while (connection->connected){
					// Get Data Recieved
					if ((inputBuffer.size = SDLNet_TCP_Recv(connection->serverSock, inputBuffer.data, 1024)) != -1){
						CBuffer buf(inputBuffer.size);
						memcpy(buf.data, inputBuffer.data, inputBuffer.size);
						mutex.lock();
						connection->inputStack.push(buf);
						mutex.unlock();
					}

					// Send Data submitted
					if (connection->outputStack.size() != 0){
						connection->mutex.lock();
						CBuffer buffOut = connection->outputStack.top();
						if (SDLNet_TCP_Send(connection->serverSock, buffOut.data, buffOut.size) != sizeof(int))
							SDL_LogError(SDL_LOG_CATEGORY_RESERVED1, "Failure in sending packets");
						delete[] buffOut.data;
						connection->outputStack.pop();
						connection->mutex.unlock();
					}
					std::this_thread::sleep_for(std::chrono::milliseconds(4));
				}
			}
		}

		void Network::serverUpdate(ServerTCPConnection* connection){
			// Await for the connection
			while ((connection->clientSock = SDLNet_TCP_Accept(connection->serverSock)) == NULL)
				std::this_thread::sleep_for(std::chrono::milliseconds(10));

			// Connected
			// Get the remote IP
			if ((connection->clientAddress = SDLNet_TCP_GetPeerAddress(connection->clientSock)))
				SDL_LogInfo(SDL_LOG_CATEGORY_RESERVED1, "Host Connected");
			else{
				SDL_LogError(SDL_LOG_CATEGORY_RESERVED1, "Host Connection Failed");
				return;
			}

			// use of mutex so we don't lock the connection threads
			connection->mutex.lock();
			connection->connected = true;
			connection->mutex.unlock();
			if (connection->connected){
				CBuffer inputBuffer(1024);
				inputBuffer.size = 0;
				while (connection->connected){
					// Get Data Recieved
					if ((inputBuffer.size = SDLNet_TCP_Recv(connection->clientSock, inputBuffer.data, 1024)) != -1){
						CBuffer buf(inputBuffer.size);
						memcpy(buf.data, inputBuffer.data, inputBuffer.size);
						mutex.lock();
						connection->inputStack.push(buf);
						mutex.unlock();
					}

					// Send Data submitted
					if (connection->outputStack.size() != 0){
						connection->mutex.lock();
						CBuffer buffOut = connection->outputStack.top();
						if (SDLNet_TCP_Send(connection->clientSock, buffOut.data, buffOut.size) != sizeof(int))
							SDL_LogError(SDL_LOG_CATEGORY_RESERVED1, "Failure in sending packets");
						delete[] buffOut.data;
						connection->outputStack.pop();
						connection->mutex.unlock();
					} 
					// Check if connection is still open


					std::this_thread::sleep_for(std::chrono::milliseconds(4));
				}
			}
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