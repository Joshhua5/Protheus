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
			TCPsocket socket; 

			//void handshake();
			void messenger();
			void listener();

			thread* _listener;
			thread* _messenger; 
		public:
			TCPConnection();
			TCPConnection(const string& name);

			//void startHandshake();

			static inline string lGetMetatable(){
				return "tcp_connection_metatable";
			}

			template<typename T>
			static inline void lGetFunctions(std::vector<luaL_Reg>& fields){ 
				CConnection::lGetFunctions<T>(fields);
			}
		};
	}
}