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
	set = SDLNet_AllocSocketSet(maxSockets);
	connections.reserve(maxSockets);

	if (!set){
		error.reportError("Unable to start server: \n" + string(SDLNet_GetError()));
		return;
	}
	listener = thread(&TCPServer::listen, this);
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

ServerTCPConnection* TCPServer::getConnection(const game_id id){
	register unsigned count = 0;
	for each(const auto con in connections)
		if (con->getGUID() == id)
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


int TCPServer::lPauseIncoming(lua_State* L){
	const auto server = Util::luaP_touserdata<TCPServer>(L, 1);
	server->pauseListening();
	return 0;
}
int TCPServer::lResumeIncoming(lua_State* L){
	const auto server = Util::luaP_touserdata<TCPServer>(L, 1);
	server->resumeListening();
	return 0;
}
int TCPServer::lGetConnectionCount(lua_State* L){
	const auto server = Util::luaP_touserdata<TCPServer>(L, 1);
	luaP_pushnumber(L, server->getConnectionCount());
	return 1;
}
int TCPServer::lGetConnection(lua_State* L){
	const auto server = Util::luaP_touserdata<TCPServer>(L, 1);
	Util::luaP_newobject(L, server->getConnections().at(luaP_togameid(L, 2)));
	return 1; 
}
int TCPServer::lGetNewConnection(lua_State* L){
	const auto server = Util::luaP_touserdata<TCPServer>(L, 1);
	Util::luaP_newobject(L, server->newConnection());
	return 1;
}
int TCPServer::lPeek(lua_State* L){
	const auto server = Util::luaP_touserdata<TCPServer>(L, 1);
	luaP_pushnumber(L, server->peek());
	return 1;

}
int TCPServer::lRecv(lua_State* L){
	const auto server = Util::luaP_touserdata<TCPServer>(L, 1);
	Util::luaP_newobject(L, server->getPendingConnection());
	return 1;
}