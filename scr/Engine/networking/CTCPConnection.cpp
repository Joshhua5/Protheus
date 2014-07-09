#include "CTCPConnection.h"

using namespace Pro;
using namespace Networking;

void TCPConnection::handshake(TCPConnection* connection, thread** listener, thread** messenger){
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
	*listener = new thread(&TCPConnection::listener, this, connection);
	*messenger = new thread(&TCPConnection::messenger, this, connection);
}
void TCPConnection::messenger(TCPConnection* connection){
	// Get data recieved
	CBuffer inputBuffer(1024);
	inputBuffer.size = 0;
	SDLNet_SocketSet set = SDLNet_AllocSocketSet(1);
	SDLNet_TCP_AddSocket(set, connection->clientSock);
	// Checks sockets for connectivity or error
	while ((SDLNet_CheckSockets(set, 0) != -1) && (SDLNet_SocketReady(connection->clientSock) != -1)){
		// Send Data submitted
		if (!connection->outputStack.empty()){
			connection->mutex.lock();
			CBuffer buffOut = connection->outputStack.top();
			if (SDLNet_TCP_Send(connection->serverSock, buffOut.data, buffOut.size) < static_cast<int>(buffOut.size)){
				// Connection is closed or error occured
				SDL_LogError(SDL_LOG_CATEGORY_RESERVED1, SDL_GetError());
				return;
			}
			delete[] buffOut.data;
			connection->outputStack.pop();
			connection->mutex.unlock();
		}
	}
}
void TCPConnection::listener(TCPConnection* connection){
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

void TCPConnection::startHandshake(){
	_handshaker = thread(&TCPConnection::handshake, this, this, &_listener, &_messenger);
}