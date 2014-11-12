#include "BufferReader.h"

using namespace Pro;
using namespace Util;

BufferReader::BufferReader(CBuffer* _buffer){
	m_buffer = _buffer;
}

BufferReader::~BufferReader(){
	m_buffer = nullptr;
}

inline char* BufferReader::read_raw() const{
	return m_buffer->data<char>() + m_head;
}

CBuffer BufferReader::read(const int size){ 
	CBuffer out(read_raw(), size, true);
	skip(size);
	return move(out);
}

CBuffer BufferReader::read_delim(const char deliminator){
	return read(find(deliminator));
}

// reads until a null terminator is found '\0'
CBuffer BufferReader::read_string(){
	return read_delim('\0');
}
 