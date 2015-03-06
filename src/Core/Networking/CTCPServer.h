/*************************************************************************
Protheus Source File.
Copyright (C), Protheus Studios, 2013-2015.
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

		class TCPServer 
		{ 
			TCPsocket serverSock; 
			connection_id id_;
			 
			SDLNet_SocketSet set;
			atomic<bool> listening;
			atomic<int> connection_count;

			vector<ServerTCPConnection*> connections;
			stack<ServerTCPConnection*> newConnections;

			void listen(); 
			thread listener;
		public:  
			TCPServer(const connection_id); 
			~TCPServer();

			connection_id id() {
				return id_;
			}

			// Initialize the server
			bool startListening(IPaddress socket, const unsigned maxSockets);

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

			ServerTCPConnection* getConnection(const connection_id);

			// returns a connection which has recieved data
			ServerTCPConnection* getPendingConnection();

			// returns the amount of connections
			// with data to be read
			unsigned peek(); 
		};
	}
}
