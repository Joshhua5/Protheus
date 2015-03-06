/*************************************************************************
Protheus Source File.
Copyright (C), Protheus Studios, 2013-2015.
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
#include <Buffer.h>

typedef int connection_id;

namespace Pro{
	namespace Networking{
		using namespace std;
		using namespace Util;
		struct CConnection
		{
			mutex mutex;
			queue<Buffer> inputStack;
			queue<Buffer> outputStack;
			atomic<bool> connected;
			connection_id id_;
		public:
			CConnection(const connection_id _id) { id_ = _id; } 

			connection_id id() { return id_; }

			// returns the amount of bytes recieved
			Buffer recv();

			// returns the amount of bytes recieved without clearing the buffer
			unsigned peek() const;

			// returns true if the connection is active
			bool isConnected();

			// sends the buffer through the connection
			void send(Buffer& buffer); 
		};
	}
}