#include "CClientTCPConnection.h"

using namespace Pro;
using namespace Networking;

ClientTCPConnection::ClientTCPConnection() : TCPConnection("ServerTCPConnection"){ }
ClientTCPConnection::ClientTCPConnection(const string& name) : TCPConnection(name){ }