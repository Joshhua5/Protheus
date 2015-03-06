/*************************************************************************
Protheus Source File.
Copyright (C), Protheus Studios, 2013-2015.
-------------------------------------------------------------------------

Description:

-------------------------------------------------------------------------
History:
- 20:05:2014: Waring J.
*************************************************************************/

#pragma once

#include "CConnection.h"
#include <thread>

namespace Pro{
	namespace Networking{
		class TCPConnection :
			public CConnection
		{
		public:
			TCPsocket socket; 

			//void handshake();
			void messenger();
			void listener();

			thread* _listener;
			thread* _messenger; 
		public: 
			TCPConnection(const connection_id);

			//void startHandshake();
			 
		};
	}
}