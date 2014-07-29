/*************************************************************************
Protheus Source File.
Copyright (C), Protheus Studios, 2013-2014.
-------------------------------------------------------------------------

Description:
	Listens for new connections and creates a SeverTCPConnection
	for each connection found.
-------------------------------------------------------------------------
History:
- 20:05:2014: Waring J.
*************************************************************************/

#pragma once

#include "CServerTCPConnection.h"
#include <atomic>

namespace Pro{
	namespace Networking{

		using namespace std;

		class TCPServer : 
			public Component::CGUID
		{ 
			TCPsocket serverSock; 
			 
			SDLNet_SocketSet set;
			atomic<bool> listening;

			vector<ServerTCPConnection*> connections;
			stack<ServerTCPConnection*> newConnections;

			void listen(); 
			thread listener;
		public:  
			TCPServer(const string& name); 
			~TCPServer();

			// Initialize the server
			void startListening(IPaddress socket, const unsigned maxSockets);

			// Pause incomming connections
			void pauseListening();
			
			// resumes the server's listener
			// and allows more connections
			void resumeListening();

			// returns the amount of connections, connected.
			size_t getConnectionCount();

			// returns a connection which has recently connected
			// after it's been returned by newConnection, it won't be
			// returned again
			ServerTCPConnection* newConnection();

			// returns a reference to all the connections
			vector<ServerTCPConnection*>& getConnections();

			ServerTCPConnection* getConnection(const game_id);

			// returns a connection which has recieved data
			ServerTCPConnection* getPendingConnection();

			// returns the amount of connections
			// with data to be read
			unsigned peek();



			static int lPauseIncoming(lua_State*);
			static int lResumeIncoming(lua_State*);
			static int lGetConnectionCount(lua_State*);
			static int lGetConnection(lua_State*);
			static int lGetNewConnection(lua_State*);
			static int lRecv(lua_State*);
			static int lPeek(lua_State*);

			/*constexpr*/ static const char* lGetMetatable(){
				return "tcp_server_metatable";
			}
			 
			template<typename T>
			static inline void lGetFunctions(std::vector<luaL_Reg>& fields){ 
				fields.push_back({ "pause", &T::lPauseIncoming });
				fields.push_back({ "resume", &T::lResumeIncoming });
				fields.push_back({ "getConnectionCount", &T::lGetConnectionCount });
				fields.push_back({ "getConnection", &T::lGetConnection });
				fields.push_back({ "getNewConnection", &T::lGetNewConnection });
				fields.push_back({ "recv", &T::lRecv });
				fields.push_back({ "peek", &T::lPeek }); 
			}
		};
	}
}
