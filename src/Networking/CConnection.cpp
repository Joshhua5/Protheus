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
