#include "CServerTCPConnection.h"

using namespace Pro;
using namespace Networking;
 
ServerTCPConnection::ServerTCPConnection(const string& name) : TCPConnection(name){ }

void ServerTCPConnection::startCommunication(){ 
	// Create listener and messenger thread
	connected.store(true);
	_listener = new thread(&TCPConnection::listener, this);
	_messenger = new thread(&TCPConnection::messenger, this);
}