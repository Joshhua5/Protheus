#include "CConnection.h"

using namespace Pro;
using namespace Networking;

CBuffer CConnection::recv(){
	const auto buffer = inputStack.top();
	inputStack.pop();
	return buffer;
}

void CConnection::send(CBuffer& _buffer){
	// clones the buffer to be sent
	CBuffer buf(_buffer);
	mutex.lock();
	outputStack.push(buf);
	mutex.unlock();
}

unsigned CConnection::peek(){
	if (inputStack.empty())
		return 0;
	return inputStack.top().size();
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