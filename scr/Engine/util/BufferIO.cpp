
#include "BufferIO.h"

using namespace Pro;
using namespace Util;


// sets the position of the writer
void BufferIO::setPosition(unsigned int position){
	head = position;
}
// returns the position of the writer
unsigned int BufferIO::getPosition(){
	return head;
}
// skips over the current writing position
void BufferIO::skip(int position){
	head += position;
}
// get's the internal buffers size
unsigned int BufferIO::getBufferSize(){
	return buffer->size;
}