#include "CTCPServer.h"

using namespace Pro;
using namespace Networking;
using namespace std;

TCPServer::TCPServer(const string& name) : Component::CGUID(name)
{
}

TCPServer::~TCPServer()
{
	listening.store(false);
}


void TCPServer::listen(){
	TCPsocket incoming = NULL;
	while (listening.load()){
		if ((incoming = SDLNet_TCP_Accept(serverSock)) == NULL)
			this_thread::sleep_for(chrono::milliseconds(20));

		// Connection found 
		auto con = new ServerTCPConnection("TCPConnection");
		con->socket = incoming;
		con->startCommunication(); 

		connections.push_back(con);
		newConnections.push(con);
	}
}

void TCPServer::startListening(IPaddress serverAddress, const unsigned maxSockets){
	serverSock = SDLNet_TCP_Open(&serverAddress);

	auto set = SDLNet_AllocSocketSet(maxSockets);
	connections.reserve(maxSockets);
	if (!set){
		error.reportError("Unable to start server: " + string(SDLNet_GetError()));
		return;
	}
	listener = thread(&TCPServer::listen, this);
}