
#include "BufferWriter.h"

using namespace Pro;
using namespace Util;

BufferWriter::BufferWriter(CBuffer* _buffer){
	buffer = _buffer;
} 

BufferWriter::~BufferWriter()
{
	buffer = nullptr;
}

void BufferWriter::write(void* value, unsigned int size){
	if (buffer == nullptr)
		throw "NullBuffer";
	if (getPosition() > buffer->size - size)
		throw "BufferOverFlow";

	memcpy(&static_cast<char*>(buffer->data)[head], value, size); 
	skip(size);
} 