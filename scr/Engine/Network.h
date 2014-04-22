#pragma once
#include <SDL_net.h>
#include <string>
#include <stack>
#include <vector>
#include <thread>
#include <mutex>
namespace Pro{

#define port 9910  

	struct Buffer{
		void* buffer;
		unsigned int size;
	};

	struct TCPConnection{
		TCPConnection(){
			isServer = false;
			connected = false;
			serverAddress = new IPaddress(); 
		}
		std::mutex mutex;
		TCPsocket serverSock;
		TCPsocket clientSock;
		std::stack<Buffer> inputStack;
		std::stack<Buffer> outputStack;
		IPaddress* serverAddress;
		IPaddress* clientAddress;
		bool isServer;
		bool connected;
	};

	class Network
	{
	private:
		std::mutex mutex;
		std::vector<TCPConnection*> connections;
		TCPConnection* _server;

		void connectionUpdate(TCPConnection* connection);
		void connect();

	public:
		Network();
		~Network();
		bool init();
		void closeAll();
		TCPConnection* startServer();
		TCPConnection* connectToServer(const std::string &IP);

		// returns the amount of bytes recieved
		unsigned int recv(TCPConnection*, void *buffer);
		// returns the amount of bytes recieved without clearing the buffer
		unsigned int peek(TCPConnection*);
		// sends the buffer through the connection
		void send(TCPConnection* connection, void* buffer, unsigned int bufferSize);
		/* sends the buffer through the connection and deletes the buffer
		// can't release arrays */
		void sendd(TCPConnection* connection, void* buffer, unsigned int bufferSize);

		void cleanup();
	};

}