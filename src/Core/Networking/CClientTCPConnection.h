#pragma once
/*************************************************************************
Protheus Source File.
Copyright (C), Protheus Studios, 2013-2014.
-------------------------------------------------------------------------

Description:
 Connecting to server
SDLNet_ResolveHost(serverIP, server, port)
SDLNet_UDP_Open(port)
SDLNet_AllocSocketSet
SDLNet_TCP_AddSocket
-------------------------------------------------------------------------
History:
- 07:06:2014: Waring J.
*************************************************************************/

#pragma once

#include "CTCPConnection.h"

using namespace std;

namespace Pro{
	namespace Networking{
		struct ClientTCPConnection :
			public TCPConnection
		{
			void handshake(IPaddress address);
			 
			thread _handshaker; 
		public: 
			ClientTCPConnection(const connection_id id); 
			  
			void startHandshake(IPaddress );
			 
		};
	}
}