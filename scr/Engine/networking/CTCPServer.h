#pragma once

#include "CServerTCPConnection.h"

namespace Pro{
	namespace Networking{
		class TCPServer
		{ 
			std::vector<ServerTCPConnection> connections;
			void listener(TCPServer*);
		public:
			TCPServer();
			~TCPServer(); 
		}; 
	}
}
