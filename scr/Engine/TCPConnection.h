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
#include <stack>
#include "CBuffer.h"
#include "CGUID.h"
#include <vector>
#include <mutex>

namespace Pro{
	namespace Networking{
		struct TCPConnection :
			public Component::CGUID 
		{
			TCPConnection() : CGUID("TCPConnection"){
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
	}
}