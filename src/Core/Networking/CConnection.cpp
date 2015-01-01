#include "CConnection.h"

using namespace Pro;
using namespace Networking;

Buffer CConnection::recv(){
	Buffer buffer = inputStack.front();
	inputStack.pop();
	return move(buffer);
}

void CConnection::send(Buffer& _buffer){
	// clones the buffer to be sent
	Buffer buf(_buffer);
	mutex.lock();
	outputStack.push(buf);
	mutex.unlock();
}

bool CConnection::isConnected(){
	return connected.load() ? true : false;
}

unsigned CConnection::peek() const{
	if (inputStack.empty())
		return 0;
	return inputStack.front().size();
}
