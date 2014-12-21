#include "CTCPConnection.h"

using namespace Pro;
using namespace Networking;

 
TCPConnection::TCPConnection(const connection_id id) : CConnection(id){ }
 
void TCPConnection::messenger(){
	// Get data recieved 
	const auto set = SDLNet_AllocSocketSet(1); 
	SDLNet_TCP_AddSocket(set, socket);

	// Checks sockets for connectivity or error

	while ((SDLNet_CheckSockets(set, 0) != -1) &&
		(SDLNet_SocketReady(socket) != -1) &&
		connected.load()){

		// Send Data submitted
		if (!outputStack.empty()){
			mutex.lock();
			// Grab the message
			Buffer buffOut = outputStack.front();
			outputStack.pop();
			mutex.unlock();

			if (SDLNet_TCP_Send(socket, buffOut.data(), buffOut.size()) < (int) buffOut.size()){
				// Connection is closed or error occured 
				error.reportError("Messenger Failed: \n" + string(SDLNet_GetError()));
				connected.store(false);
				SDLNet_FreeSocketSet(set);
				return;
			}
		}
	}
	connected.store(false);
	SDLNet_FreeSocketSet(set);
}
void TCPConnection::listener(){
	// Get data recieved
	const int INPUT_BUFFER_SIZE = 1024;
	Buffer inputBuffer(INPUT_BUFFER_SIZE);
	unsigned bufferSize = 0;
	while ((bufferSize = SDLNet_TCP_Recv(socket, inputBuffer.data(), INPUT_BUFFER_SIZE)) > 0 &&
		connected.load()){
		// Get Data Recieved
		Buffer buf(inputBuffer.data(), bufferSize, true);
		mutex.lock();
		inputStack.push(move(buf));
		mutex.unlock();
	}
	connected.store(false);
}

