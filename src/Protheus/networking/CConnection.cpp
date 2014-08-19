#include "CConnection.h"

using namespace Pro;
using namespace Networking;

CBuffer CConnection::recv(){
	const auto buffer = inputStack.front();
	inputStack.pop();
	return move(buffer);
}

void CConnection::send(CBuffer& _buffer){
	// clones the buffer to be sent
	CBuffer buf(_buffer);
	mutex.lock();
	outputStack.push(buf);
	mutex.unlock();
}

bool CConnection::isConnected(){
	return connected.load() ? true : false;
}

unsigned CConnection::peek(){
	if (inputStack.empty())
		return 0;
	return inputStack.front().size();
}

int CConnection::lSend(lua_State* L){
	const auto c = Util::luaP_touserdata<CConnection>(L, 1); 
	c->send(*Util::luaP_touserdata<CBuffer>(L, 2));
	return 0;
}

int CConnection::lPeek(lua_State* L){
	const auto c = Util::luaP_touserdata<CConnection>(L, 1);
	lua_pushnumber(L, c->peek());
	return 1;
}

int CConnection::lRecv(lua_State* L){
	const auto c = Util::luaP_touserdata<CConnection>(L, 1); 
	Util::luaP_newobject(L, c->recv());
	return 1;
}

int CConnection::lIsConnected(lua_State* L){
	const auto c = Util::luaP_touserdata<CConnection>(L, 1);
	lua_pushboolean(L, c->isConnected());
	return 1;
}