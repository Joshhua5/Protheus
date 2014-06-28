#include "BufferReader.h"


BufferReader::BufferReader(CBuffer* _buffer){
	buffer = _buffer;
} 

BufferReader::~BufferReader(){
	buffer = nullptr;
}

inline char* BufferReader::read_raw(){
	return &static_cast<char*>(buffer->data)[head];
}

char* BufferReader::read(int size){
	char* out = new char[size];
	memcpy(out, read_raw(), size);
	skip(size);
	return out;
}
 