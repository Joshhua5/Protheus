#include "CNetwork.h"
using namespace Pro;
using namespace Networking;
Network::Network()
{
	if (SDLNet_Init() < 0)
		SDL_LogError(SDL_LOG_CATEGORY_CUSTOM, "Network Init Failure");
}

Network::~Network(){
	// close all client connections
	connections.erase(connections.begin(), connections.end());
	SDLNet_Quit();
}

TCPServer* Network::startServer(){
	return startServer("", 9910, 128);
}

TCPServer* Network::startServer(const unsigned short port){
	return startServer("", port, 128);
}

TCPServer* Network::startServer(const string& name, const unsigned short port, const unsigned maxConnection){
	// Create Connection
	TCPServer* _connection = new TCPServer(name);
	IPaddress add;
	// Check the address for open socket to connect to
	if (SDLNet_ResolveHost(&add, NULL, port)){
		error.reportError("Unable to start server: " + name + "\n" + SDLNet_GetError());
		return nullptr;
	}
	// start trying to connect to the server 
	_connection->startListening(add, maxConnection);
	return _connection;
}

ClientTCPConnection* Network::connectToServer(const std::string &IP){
	return connectToServer(IP, "", 9910);
}

ClientTCPConnection*  Network::connectToServer(const std::string &IP, const unsigned short port){
	return connectToServer(IP, "", port);
}

ClientTCPConnection*  Network::connectToServer(const std::string &IP, const string& name, const unsigned short port){
	// Create Connection
	ClientTCPConnection* _connection = new ClientTCPConnection(name); 
	IPaddress address;
	// Check the address for open socket to connect to
	if (SDLNet_ResolveHost(&address, IP.c_str(), port)){
		error.reportError("Unable to start client: " + name + "\n" + SDLNet_GetError());
		return nullptr;
	}
	// start trying to connect to the server
	_connection->startHandshake(address);
	connections.push_back(_connection);
	return _connection;
}


int Network::lStartServer(lua_State* L){
	static const auto n = Util::luaP_touserdata<Network>(L, 1);
	const auto s = n->startServer(lua_tostring(L, 2), luaP_toushort(L, 3), luaP_touint(L, 4));
	Util::luaP_newobject<TCPServer>(L, s);
	return 1;
}

int Network::lConnectToServer(lua_State* L){
	static const auto n = Util::luaP_touserdata<Network>(L, 1);
	const auto c = n->connectToServer(lua_tostring(L, 2), lua_tostring(L, 3), luaP_toushort(L, 4));
	Util::luaP_newobject<ClientTCPConnection>(L, c);
	return 1;
}
