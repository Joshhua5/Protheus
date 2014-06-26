#include "BufferWriter.h"


CBufferWriter::CBufferWriter(CBuffer& _buffer){
	buffer = &_buffer;
}
CBufferWriter::CBufferWriter(CBuffer* _buffer){
	buffer = _buffer;
} 

CBufferWriter::~CBufferWriter()
{
}

void CBufferWriter::write(void* value, int size){
	if (buffer == nullptr)
		throw "NullBuffer";
	if (getPosition() > buffer->size - size)
		throw "BufferOverFlow";

	memcpy(&static_cast<char*>(buffer->data)[writerPosition], value, size); 
	skip(size);
}

void CBufferWriter::setPosition(unsigned int pos){
	if (pos > buffer->size)
		throw "BufferOverFlow";
	writerPosition = pos;
}

unsigned int CBufferWriter::getPosition(){
	return writerPosition;
}

void CBufferWriter::skip(int delta){
	setPosition(getPosition() + delta);
}