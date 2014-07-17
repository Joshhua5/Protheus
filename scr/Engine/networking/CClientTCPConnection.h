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
			ClientTCPConnection();
			ClientTCPConnection(const string& name); 
			  
			void startHandshake(IPaddress );

			static inline string lGetMetatable(){
				return "c_tcp_connection_metatable";
			}

			template<typename T>
			static inline void lGetFunctions(std::vector<luaL_Reg>& fields){
				TCPConnection::lGetFunctions<T>(fields);
			}
		};
	}
}