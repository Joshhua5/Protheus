/*************************************************************************
Protheus Source File.
Copyright (C), Protheus Studios, 2013-2014.
-------------------------------------------------------------------------

Description:

-------------------------------------------------------------------------
History:
- 20:05:2014: Waring J.
*************************************************************************/

#pragma once
#include <SDL_net.h>
#include <string>
#include <stack>
#include <vector>
#include <thread>
#include <mutex>
#include "CBuffer.h"
namespace Pro{
	namespace Networking{
#define port 9910  
		 
		struct TCPConnection{
			TCPConnection(){
				isServer = false;
				connected = false;
				serverAddress = new IPaddress();
			}
			std::mutex mutex;
			TCPsocket serverSock;
			TCPsocket clientSock;
			std::stack<CBuffer> inputStack;
			std::stack<CBuffer> outputStack;
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
			unsigned int recv(TCPConnection&, CBuffer& buffer);
			// returns the amount of bytes recieved without clearing the buffer
			unsigned int peek(TCPConnection&);
			// sends the buffer through the connection
			void send(TCPConnection& connection, CBuffer& buffer); 

			void cleanup();
		}; 
	}
}