#include "BufferReader.h"

using namespace std;
using namespace Pro;
using namespace Util;


BufferReader::BufferReader(BufferReader&& buffer) {
	m_buffer = buffer.m_buffer;
	m_head = buffer.m_head;
	buffer.m_buffer = nullptr;
}

BufferReader::BufferReader(CBuffer* _buffer) {
	m_buffer = _buffer;
}

BufferReader::~BufferReader() {
	m_buffer = nullptr;
}

inline char* BufferReader::read_raw() const {
	return m_buffer->data<char>() + m_head;
}

CBuffer BufferReader::read(const unsigned size, bool copy) {
	if ( m_head - m_buffer->size() <= size)
		return CBuffer(0);
	CBuffer out(read_raw(), size, copy);
	skip(size);
	return move(out);
}

CBuffer BufferReader::read_delim(const char deliminator, bool copy) {
	return read(find(deliminator), copy);
}

// reads until a null terminator is found '\0'
string BufferReader::read_string() {
	return string(read_delim('\0').data<char>());
}

string BufferReader::read_string(const unsigned size) { 
	char* arr = read_array<char>(size + 1);
	skip(-1);
	arr[size] = '\0';
	string out(arr);
	delete [] arr;
	return std::move(out);
}


bool BufferReader::hasNext() { 
	return (m_head < m_buffer->size()) ? true : false;
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
