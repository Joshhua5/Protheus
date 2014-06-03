#pragma once
#include "TCPConnection.h"

namespace Pro{
	namespace Networking{
		class TCPServer
		{ 
			std::vector<TCPConnection> connections;
			void listener(TCPServer&);
		public:
			TCPServer();
			~TCPServer();


		}; 
	}
}
