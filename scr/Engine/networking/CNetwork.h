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

#include "..\util\CBuffer.h"
#include "CServerTCPConnection.h"
#include "CClientTCPConnection.h"
#include "CTCPServer.h"
#include <string>

namespace Pro{
	namespace Networking{
		class Network
		{
		private:
			std::mutex mutex;
			std::vector<CConnection*> connections;

			void connect();

		public:
			Network();
			~Network();

			TCPServer* startServer();
			TCPServer* startServer(const unsigned short port);
			TCPServer* startServer(const string& name, const unsigned short port, const unsigned maxConnections);

			ClientTCPConnection* connectToServer(const std::string &IP);
			ClientTCPConnection* connectToServer(const std::string &IP, const unsigned short port);
			ClientTCPConnection* connectToServer(const std::string &IP, const string& name, const unsigned short port);
			  
			static int lStartServer(lua_State*);
			static int lConnectToServer(lua_State*);

			static inline string lGetMetatable(){
				return "network_metatable";
			}

			template<typename T>
			static inline void lGetFunctions(std::vector<luaL_Reg>& fields){ 
				fields.push_back({ "startServer", &T::lStartServer });
				fields.push_back({ "connectToServer", &T::lConnectToServer });
			}
		};
	}
}