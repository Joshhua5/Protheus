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
 
#include "SDL_net/SDL_net.h"
#include <queue>
#include <stack>
#include <mutex>
#include <atomic>
#include <BufferReader.h>
#include <CBuffer.h>

typedef int connection_id;

namespace Pro{
	namespace Networking{
		using namespace std;
		struct CConnection
		{
			mutex mutex;
			queue<CBuffer> inputStack;
			queue<CBuffer> outputStack;
			atomic<bool> connected;
			connection_id id;
		public:
			CConnection(const connection_id _id) { id = _id; } 

			connection_id getID() { return id; }

			// returns the amount of bytes recieved
			CBuffer recv();

			// returns the amount of bytes recieved without clearing the buffer
			unsigned int peek();

			// returns true if the connection is active
			bool isConnected();

			// sends the buffer through the connection
			void send(CBuffer& buffer); 
		};
	}
}