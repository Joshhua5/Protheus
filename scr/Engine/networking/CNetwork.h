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

#include "..\util\CBuffer.h"
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
			  
			void connect();

		public:
			Network();
			~Network();
			bool init();
			void closeAll();
			ServerTCPConnection* startServer();
			ServerTCPConnection* startServer(int port);
			ServerTCPConnection* startServer(const string* name, int port);
			
			ClientTCPConnection* connectToServer(const std::string &IP);
			ClientTCPConnection* connectToServer(const std::string &IP, int port);
			ClientTCPConnection* connectToServer(const std::string &IP, const string& name, int port);
			 
			void cleanup();
		}; 
	}
} 