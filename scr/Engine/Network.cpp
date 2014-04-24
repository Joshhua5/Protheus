#include "Network.h"
namespace Pro{
	namespace Networking{

		Network::Network()
		{
			_server = nullptr;

		}


		Network::~Network()
		{
		}

		void Network::connectionUpdate(TCPConnection* connection){
			// wait for connection
			if (connection->isServer){
				while ((connection->clientSock = SDLNet_TCP_Accept(connection->serverSock)) == NULL){
					std::this_thread::sleep_for(std::chrono::milliseconds(10));
				}
			}
			else
			{
				while (!(connection->serverSock = SDLNet_TCP_Open(connection->serverAddress))){
					std::this_thread::sleep_for(std::chrono::milliseconds(10));
				}
			}

			// Connected
			// Get the remote IP
			if ((connection->clientAddress = SDLNet_TCP_GetPeerAddress(connection->clientSock)))
				SDL_LogInfo(SDL_LOG_CATEGORY_RESERVED1, "Host Connected");
			else
				SDL_LogError(SDL_LOG_CATEGORY_RESERVED1, "Host Connection Failed");
			connection->mutex.lock();
			connection->connected = true;
			connection->mutex.unlock();

			// Get data recieved
			char* inputBuffer = new char[1024];
			unsigned int inputBufferSize = 0;
			while (connection->connected){
				// Get Data Recieved
				if (connection->isServer){
					if ((inputBufferSize = SDLNet_TCP_Recv(connection->clientSock, inputBuffer, 1024)) != -1){
						Buffer buf;
						buf.buffer = new char[inputBufferSize];
						buf.size = inputBufferSize;
						memcpy(buf.buffer, inputBuffer, inputBufferSize);
						mutex.lock();
						connection->inputStack.push(buf);
						mutex.unlock();
					}
				}
				else
				{
					if ((inputBufferSize = SDLNet_TCP_Recv(connection->serverSock, inputBuffer, 1024)) != -1){
						Buffer buf;
						buf.buffer = new char[inputBufferSize];
						buf.size = inputBufferSize;
						memcpy(buf.buffer, inputBuffer, inputBufferSize);
						mutex.lock();
						connection->inputStack.push(buf);
						mutex.unlock();
					}
				}

				// Send Data submitted
				if (connection->outputStack.size() != 0){
					Buffer buffOut = connection->outputStack.top();
					if (connection->isServer){
						if (SDLNet_TCP_Send(connection->clientSock, buffOut.buffer, buffOut.size) != sizeof(int))
							SDL_LogError(SDL_LOG_CATEGORY_RESERVED1, "Failure in sending packets");
					}
					else
						if (SDLNet_TCP_Send(connection->serverSock, buffOut.buffer, buffOut.size) != sizeof(int))
							SDL_LogError(SDL_LOG_CATEGORY_RESERVED1, "Failure in sending packets");

					delete[] buffOut.buffer;
					connection->outputStack.pop();
				}
				std::this_thread::sleep_for(std::chrono::milliseconds(4));
			}
			delete[] inputBuffer;
			return;
		}


		bool Network::init(){
			if (SDLNet_Init() < 0){
				SDL_LogError(SDL_LOG_CATEGORY_CUSTOM, "Network Init Failure");
				return false;
			}
			return true;
		}

		TCPConnection* Network::startServer(){
			_server = new TCPConnection();
			_server->isServer = true;
			if (SDLNet_ResolveHost(_server->serverAddress, NULL, port)){
				SDL_LogError(SDL_LOG_CATEGORY_CUSTOM, "Network Listen Failure");
				delete _server;
				return false;
			}
			if (!(_server->serverSock = SDLNet_TCP_Open(_server->serverAddress))){
				delete _server;
				return false;
			}
			// Start listening for a port
			std::thread* thread = new std::thread(&Network::connectionUpdate, this, _server);
			thread->detach();
			return _server;
		}

		TCPConnection* Network::connectToServer(const std::string &IP){
			TCPConnection* _connection = new TCPConnection;

			if (SDLNet_ResolveHost(_connection->serverAddress, IP.c_str(), port)){
				SDL_LogError(SDL_LOG_CATEGORY_RESERVED1, "Network Listen Failure");
				return false;
			}
			// start trying to connect to the server
			std::thread* thread = new std::thread(&Network::connectionUpdate, this, _connection);
			thread->detach();
			connections.push_back(_connection);
			return _connection;
		}

		unsigned int Network::recv(TCPConnection* _connection, void *buffer){
			Buffer buf = _connection->inputStack.top();
			_connection->inputStack.pop();
			buffer = buf.buffer;
			return buf.size;
		}
		// send buffer
		void Network::send(TCPConnection* _connection, void* _buffer, unsigned int _size){
			Buffer buffOut;
			// make copy of buffer
			buffOut.buffer = new char[_size];
			memcpy(buffOut.buffer, _buffer, _size);
			buffOut.size = _size;
			_connection->mutex.lock();
			_connection->outputStack.push(buffOut);
			_connection->mutex.unlock();
		}
		void Network::closeAll(){
			if (_server != nullptr)
				_server->connected = false;
			for (unsigned int x = 0; x < connections.size(); x++){
				connections[x]->connected = false;
			}
		}

		void Network::cleanup(){
			// close the server
			delete _server;
			// close all client connections
			while (!connections.empty())
				connections.erase(connections.begin());
			SDLNet_Quit();
		}

		void Network::sendd(TCPConnection* connection, void* buffer, unsigned int bufferSize){
			send(connection, buffer, bufferSize);
			delete buffer;
		}

		unsigned int Network::peek(TCPConnection* con){
			return con->inputStack.top().size;
		}
	}
}