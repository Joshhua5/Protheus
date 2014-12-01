#include "BufferReader.h"

using namespace Pro;
using namespace Util;

BufferReader::BufferReader(CBuffer* _buffer) {
	m_buffer = _buffer;
}

BufferReader::~BufferReader() {
	m_buffer = nullptr;
}

inline char* BufferReader::read_raw() const {
	return m_buffer->data<char>() + m_head;
}

CBuffer BufferReader::read(const unsigned size) {
	if ( m_head - m_buffer->size() <= size)
		return CBuffer(0);
	CBuffer out(read_raw(), size, true);
	skip(size);
	return move(out);
}

CBuffer BufferReader::read_delim(const char deliminator) {
	return read(find(deliminator));
}

// reads until a null terminator is found '\0'
CBuffer BufferReader::read_string() {
	return read_delim('\0');
}

long BufferReader::read_bits(const unsigned bits) {
	// TEST
	if (bits < 8)
		return read<char>() & static_cast<unsigned>((pow(2, bits) - 1));
	else if (bits < 16)
		return read<short>() & static_cast<unsigned>((pow(2, bits) - 1));
	else if (bits < 32)
		return read<int>() & static_cast<unsigned>((pow(2, bits) - 1));
	else if (bits < 64)
		return read<long>() & static_cast<unsigned>((pow(2, bits) - 1));
	return 0;
}
