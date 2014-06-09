#include "CServerTCPConnection.h"

using namespace Pro;
using namespace Networking;

ServerTCPConnection::ServerTCPConnection() : TCPConnection("ServerTCPConnection"){
	serverAddress = new IPaddress();
}
ServerTCPConnection::ServerTCPConnection(const string& name) : TCPConnection(name){
	serverAddress = new IPaddress();
}
void ServerTCPConnection::handshake(ServerTCPConnection* connection, thread & listener, thread & messenger){
	// Await for the connection
	while ((connection->clientSock = SDLNet_TCP_Accept(connection->serverSock)) == NULL)
		std::this_thread::sleep_for(std::chrono::milliseconds(10));
	// Connected
	// Get the remote IP
	if ((connection->clientAddress = SDLNet_TCP_GetPeerAddress(connection->clientSock)))
		SDL_LogInfo(SDL_LOG_CATEGORY_RESERVED1, "Host Connected");
	else{
		SDL_LogError(SDL_LOG_CATEGORY_RESERVED1, "Host Connection Failed");
		return;
	}
	 
	// Create listener and messenger thread
	listener = thread(&ServerTCPConnection::listener, connection);
	messenger = thread(&ServerTCPConnection::messenger, connection);
}

void ServerTCPConnection::messenger(ServerTCPConnection* connection){ 
		CBuffer inputBuffer(1024);
		inputBuffer.size = 0;
		SDLNet_SocketSet set;
		SDLNet_TCP_AddSocket(set, connection->clientSock);
		// Checks sockets for connectivity or error
		while ((SDLNet_CheckSockets(set, 0) != -1) && (SDLNet_SocketReady(connection->clientSock) != -1)){ 
			// Send Data submitted
			if (!connection->outputStack.empty()){
				connection->mutex.lock();
				CBuffer buffOut = connection->outputStack.top();
				if (SDLNet_TCP_Send(connection->clientSock, buffOut.data, buffOut.size) < buffOut.size){
					// Connection is closed or error occured
					SDL_LogError(SDL_LOG_CATEGORY_RESERVED1, SDL_GetError());
					return;
				}
				delete [] buffOut.data;
				connection->outputStack.pop();
				connection->mutex.unlock();
			} 
	}
}
void ServerTCPConnection::listener(ServerTCPConnection* connection){
	CBuffer inputBuffer(1024);
	inputBuffer.size = 0;
	while ((inputBuffer.size = SDLNet_TCP_Recv(connection->clientSock, inputBuffer.data, 1024)) > 0){
		// Get Data Recieved 
		CBuffer buf(inputBuffer.data, inputBuffer.size); 
		mutex.lock();
		connection->inputStack.push(buf);
		mutex.unlock();
	} 
}