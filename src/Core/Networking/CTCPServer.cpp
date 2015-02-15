#include "CTCPServer.h"

using namespace Pro;
using namespace Networking;
using namespace std;

TCPServer::TCPServer(const connection_id id)
{
}

TCPServer::~TCPServer()
{
	listening.store(false);
}


void TCPServer::listen(){
	TCPsocket incoming = nullptr;
	while (true){
		if (!listening.load()){
			this_thread::sleep_for(chrono::milliseconds(100));
			continue;
		}
		if ((incoming = SDLNet_TCP_Accept(serverSock)) == nullptr)
			this_thread::sleep_for(chrono::milliseconds(20));
		else{ 
			// Connection found 
			auto con = new ServerTCPConnection(connection_count++); 
			con->socket = incoming;
			con->startCommunication();

			connections.push_back(con);
			newConnections.push(con);
		}
	}
}

bool TCPServer::startListening(IPaddress serverAddress, const unsigned maxSockets){
	serverSock = SDLNet_TCP_Open(&serverAddress);
	set = SDLNet_AllocSocketSet(maxSockets);
	connections.reserve(maxSockets);

	if (!set){
		log.Report<LogCode::ERROR>("Unable to start server: \n" + string(SDLNet_GetError()), __FUNCTION__, __LINE__);
		return false;
	}
	listening.store(true);
	listener = thread(&TCPServer::listen, this); 
	return true;
}

// Pause incomming connections
void TCPServer::pauseListening(){
	listening.store(false);
}

// resumes the server's listener
// and allows more connections
void TCPServer::resumeListening(){
	listening.store(true);
}

// returns the amount of connections, connected.
size_t TCPServer::getConnectionCount(){
	return connections.size();
}

ServerTCPConnection* TCPServer::newConnection(){
	register const auto con = newConnections.top();
	newConnections.pop();
	return con;
}

vector<ServerTCPConnection*>& TCPServer::getConnections(){
	return connections;
}


ServerTCPConnection* TCPServer::getPendingConnection(){
	for each(const auto con in connections)
		if (con->peek() != 0)
			return con;
	return nullptr;
}

ServerTCPConnection* TCPServer::getConnection(const connection_id _id){
	register unsigned count = 0;
	for each(const auto con in connections) 
		if (con->id() == _id)
			return con;

	return nullptr;
}

// returns the amount of connections
// with data to be read
unsigned TCPServer::peek(){
	register unsigned count = 0;
	for each(const auto con in connections)
		count += (con->peek() == 0) ? 0 : 1;
	return count;
}
 