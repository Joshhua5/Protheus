#include "CClientTCPConnection.h"

using namespace Pro;
using namespace Networking;

ClientTCPConnection::ClientTCPConnection() : TCPConnection("ServerTCPConnection"){
	serverAddress = new IPaddress();
}
ClientTCPConnection::ClientTCPConnection(const string& name) : TCPConnection(name){
	serverAddress = new IPaddress();
}

void ClientTCPConnection::handshake(ClientTCPConnection* connection, thread& listener, thread& messenger){
	// wait for connection 
	while (!(connection->serverSock = SDLNet_TCP_Open(connection->serverAddress)))
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
	listener = thread(&ClientTCPConnection::listener, connection);
	messenger = thread(&ClientTCPConnection::messenger, connection);

}
void ClientTCPConnection::messenger(ClientTCPConnection* connection){
	// Get data recieved 
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
			if (SDLNet_TCP_Send(connection->serverSock, buffOut.data, buffOut.size) < buffOut.size){
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
void ClientTCPConnection::listener(ClientTCPConnection* connection){
	// Get data recieved  
	CBuffer inputBuffer(1024);
	inputBuffer.size = 0;
	while ((inputBuffer.size = SDLNet_TCP_Recv(connection->serverSock, inputBuffer.data, 1024)) > 0){
		// Get Data Recieved 
		CBuffer buf(inputBuffer.size);
		memmove(buf.data, inputBuffer.data, inputBuffer.size);
		mutex.lock();
		connection->inputStack.push(buf);
		mutex.unlock(); 
	}
}