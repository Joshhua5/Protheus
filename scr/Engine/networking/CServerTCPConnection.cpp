
#include "CServerTCPConnection.h"

using namespace Pro;
using namespace Networking;

ServerTCPConnection::ServerTCPConnection() : TCPConnection("ServerTCPConnection"){ }
ServerTCPConnection::ServerTCPConnection(const string& name) : TCPConnection(name){ } 