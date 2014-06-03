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

#include "CBuffer.h"
#include "TCPConnection.h"
#include <string> 
#include <thread>
#define port 9910   
namespace Pro{
	namespace Networking{ 
		class Network
		{
		private:
			std::mutex mutex;
			std::vector<TCPConnection*> connections;
			TCPConnection* _server;
			 
			void serverUpdate(TCPConnection& connection);
			void clientUpdate(TCPConnection& connection);
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