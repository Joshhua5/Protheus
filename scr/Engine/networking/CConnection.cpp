
#include "CConnection.h"

using namespace Pro;
using namespace Networking;

unsigned int CConnection::recv(CBuffer& buffer){
	buffer = inputStack.top();
	return buffer.size;
}

void CConnection::send(CBuffer& _buffer){
	// clones the buffer to be sent
	CBuffer buf(_buffer);
	mutex.lock();
	outputStack.push(buf);
	mutex.unlock();
}

unsigned int CConnection::peek(){
	return inputStack.top().size;
}