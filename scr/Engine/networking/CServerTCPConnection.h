#pragma once
/*************************************************************************
Protheus Source File.
Copyright (C), Protheus Studios, 2013-2014.
-------------------------------------------------------------------------

Description:

-------------------------------------------------------------------------
History:
- 07:06:2014: Waring J.
*************************************************************************/

#pragma once
#include <SDL_net.h>
#include <thread>
#include "..\util\CBuffer.h"
#include "CTCPConnection.h"

using namespace std;

namespace Pro{
	namespace Networking{
		struct ServerTCPConnection :
			public TCPConnection
		{ 
			void handshake(ServerTCPConnection * connection, thread & listener, thread & messenger);
			void messenger(ServerTCPConnection* connection);
			void listener(ServerTCPConnection* connection);

			thread _handshaker;
			thread _listener;
			thread _messenger;
		public:
			ServerTCPConnection();
			ServerTCPConnection(const string& name);  
		};
	}
}