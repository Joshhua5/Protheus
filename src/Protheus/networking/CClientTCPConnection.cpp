#include "CClientTCPConnection.h"

using namespace Pro;
using namespace Networking;

ClientTCPConnection::ClientTCPConnection() : TCPConnection("ServerTCPConnection"){ }
ClientTCPConnection::ClientTCPConnection(const string& name) : TCPConnection(name){
	connected.store(false);
}



void ClientTCPConnection::handshake(IPaddress address){
	// wait for connection
	while ((socket = SDLNet_TCP_Open(&address)) == NULL)
		std::this_thread::sleep_for(std::chrono::milliseconds(10)); 

	// Create listener and messenger thread
	connected.store(true);
	_listener = new thread(&TCPConnection::listener, this);
	_messenger = new thread(&TCPConnection::messenger, this);
}


void ClientTCPConnection::startHandshake(IPaddress address){
	_handshaker = thread(&ClientTCPConnection::handshake, this, address);
}