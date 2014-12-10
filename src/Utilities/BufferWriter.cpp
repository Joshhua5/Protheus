#include "BufferWriter.h"

using namespace Pro;
using namespace Util;

BufferWriter::BufferWriter(CBuffer* _buffer){
	m_buffer = _buffer;
	m_head = 0;
}

BufferWriter::~BufferWriter()
{
	m_buffer = nullptr;
}

void BufferWriter::write(const void* value,const unsigned size){
	if (m_buffer == nullptr)
		return;

	// Check if the write will overflow
	if (m_head + size > m_buffer->size())
		// Resizes the buffer with 64 bytes overhead
		m_buffer->resize(m_head + size + 64); 

	memcpy(m_buffer->at(m_head) , value, size);
	skip(size);
}
 