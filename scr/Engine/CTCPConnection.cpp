#include "CTCPConnection.h"

using namespace Pro;
using namespace Networking;

unsigned int TCPConnection::recv(CBuffer& buffer){
	buffer = inputStack.top();
	return buffer.size;
}

void TCPConnection::send(CBuffer& _buffer){
	// clones the buffer to be sent
	CBuffer buf(_buffer);
	mutex.lock();
	outputStack.push(buf);
	mutex.unlock();
}

unsigned int TCPConnection::peek(){
	return inputStack.top().size;
}