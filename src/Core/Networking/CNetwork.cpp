#include "CNetwork.h"
#include <log.h>

using namespace Pro;
using namespace Networking;
Network::Network()
{
	if (SDLNet_Init() < 0)
		Pro::global_log.Report<LogCode::ERROR>("Network Constructor Failure" + string(SDLNet_GetError()), __FUNCTION__, __LINE__);
}

Network::~Network(){
	// close all client connections
	connections.erase(connections.begin(), connections.end());
	SDLNet_Quit();
}

TCPServer* Network::startServer(){
	return startServer(9910, 128);
}
 
TCPServer* Network::startServer(const unsigned short port, const unsigned max_connections){
	// Create Connection
	TCPServer* _connection = new TCPServer(serverID++);
	IPaddress add;
	// Check the address for open socket to connect to
	if (SDLNet_ResolveHost(&add, NULL, port)){
		global_log.Report<LogCode::ERROR>("Unable to start server: \n" + string(SDLNet_GetError()), __FUNCTION__, __LINE__);
		return nullptr;
	}
	// start trying to connect to the server 
	// return nullptr if failed to start 
	if (_connection->startListening(add, max_connections))
		return _connection;
	else{
		delete _connection;
		return nullptr;
	}

}

ClientTCPConnection* Network::connectToServer(const std::string &IP){
	return connectToServer(IP, 9910);
} 

ClientTCPConnection*  Network::connectToServer(const std::string &IP, const unsigned short port){
	// Create Connection 
	ClientTCPConnection* _connection = new ClientTCPConnection(serverID++); 
	IPaddress address;
	// Check the address for open socket to connect to
	if (SDLNet_ResolveHost(&address, IP.c_str(), port)){
		global_log.Report<LogCode::ERROR>("Unable to start client: \n" + string(SDLNet_GetError()),  __FUNCTION__, __LINE__);
		return nullptr;
	}
	// start trying to connect to the server
	_connection->startHandshake(address);
	connections.push_back(_connection); 
	return _connection;
}

 