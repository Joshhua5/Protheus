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
#include <stack>
#include "CBuffer.h"
#include "CConnection.h" 
namespace Pro{
	namespace Networking{
		struct ServerTCPConnection :
			public TCPConnection
		{ 
		public:
			ServerTCPConnection() : TCPConnection("ServerTCPConnection"){
				serverAddress = new IPaddress();
			}
			ServerTCPConnection(const string& name) : TCPConnection(name){
				serverAddress = new IPaddress();
			}

			~ServerTCPConnection(){};
		};
	}
}