#pragma once
struct CBuffer
{  
	unsigned int size; 
	void* data;
public:

	CBuffer(unsigned int _size) { size = _size; data = new char[size]; }
	CBuffer() { size = 0;}
	~CBuffer() { delete[] data; } 
};

