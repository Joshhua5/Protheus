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
		struct ClientTCPConnection :
			public TCPConnection
		{  
			void handshake(ClientTCPConnection * connection, thread & listener, thread & messenger);
			void messenger(ClientTCPConnection* connection);
			void listener(ClientTCPConnection* connection);

			thread _handshaker;
			thread _listener;
			thread _messenger;

		public:
			ClientTCPConnection();
			ClientTCPConnection(const string& name); 

			~ClientTCPConnection(){};
		};
	}
}