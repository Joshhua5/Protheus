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
#include <mutex>
#include "..\util\CBuffer.h"
#include "..\component\CGUID.h"

namespace Pro{
	namespace Networking{
		struct CConnection :
			public Component::CGUID
		{
			std::mutex mutex;
			std::stack<CBuffer> inputStack;
			std::stack<CBuffer> outputStack;

		public:
			CConnection(const string& name) : CGUID(name){};
			CConnection() : CGUID("Connection"){};
			~CConnection(){};

			// returns the amount of bytes recieved
			unsigned int recv(CBuffer& buffer);

			// returns the amount of bytes recieved without clearing the buffer
			unsigned int peek();

			// sends the buffer through the connection
			void send(CBuffer& buffer);
		};
	}
}