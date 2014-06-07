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
		public:
			TCPConnection() : CConnection("TCPConnection"){
				serverAddress = new IPaddress();
			}
			TCPConnection(const string& name) : CConnection(name){
				serverAddress = new IPaddress();
			}

			~TCPConnection(){};

			// returns the amount of bytes recieved
			unsigned int recv(CBuffer& buffer);

			// returns the amount of bytes recieved without clearing the buffer
			unsigned int peek();

			// sends the buffer through the connection
			void send(CBuffer& buffer); 
		};
	}
}