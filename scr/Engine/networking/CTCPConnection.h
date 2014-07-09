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

#include "CConnection.h"
#include <thread>

namespace Pro{
	namespace Networking{
		class TCPConnection :
			public CConnection
		{
		public:
			TCPsocket serverSock;
			TCPsocket clientSock;

			IPaddress* serverAddress;
			IPaddress* clientAddress;

			void handshake(TCPConnection* connection, thread** listener, thread** messenger);
			void messenger(TCPConnection* connection);
			void listener(TCPConnection* connection);

			thread _handshaker;
			thread* _listener;
			thread* _messenger;

		public:
			TCPConnection() : CConnection("TCPConnection"){
				serverAddress = new IPaddress();
			}
			TCPConnection(const string& name) : CConnection(name){
				serverAddress = new IPaddress();
			}

			~TCPConnection(){};

			void startHandshake();
		};
	}
}