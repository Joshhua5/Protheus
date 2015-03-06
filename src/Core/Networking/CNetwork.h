/*************************************************************************
Protheus Source File.
Copyright (C), Protheus Studios, 2013-2015.
-------------------------------------------------------------------------

Description:

-------------------------------------------------------------------------
History:
- 20:05:2014: Waring J.
*************************************************************************/

#pragma once

#include <Buffer.h>
#include "CServerTCPConnection.h"
#include "CClientTCPConnection.h"
#include "CTCPServer.h"
#include <string>

namespace Pro{
	namespace Networking{
		class Network
		{
		private:
			std::mutex mutex;
			std::vector<CConnection*> connections; 
			atomic<unsigned> serverID = 0;
			void connect();

		public:
			Network();
			~Network();

			TCPServer* startServer();
			TCPServer* startServer(const unsigned short port, const unsigned max_connections); 

			ClientTCPConnection* connectToServer(const std::string &IP);
			ClientTCPConnection* connectToServer(const std::string &IP, const unsigned short port); 
		 
		};
	}
}