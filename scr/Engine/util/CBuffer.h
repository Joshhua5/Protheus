/*************************************************************************
Protheus Source File.
Copyright (C), Protheus Studios, 2013-2014.
-------------------------------------------------------------------------

Description:
	A structure to hold a pointer to a buffer, also stores the size of this
	buffer. The constructor will define the size in bytes and allocate the
	buffer if a size is provided.
	
-------------------------------------------------------------------------
History:
- 1:06:2014: Waring J.
*************************************************************************/
#pragma once

#include <memory>

struct CBuffer
{  
	unsigned int size; 
	void* data;
public:
	CBuffer(void* _data, unsigned int _size) : CBuffer(_size){ memcpy(data, _data, _size);  }
	CBuffer(unsigned int _size) { size = _size; data = new char[size]; }
	CBuffer() { size = 0; data = nullptr; }
	~CBuffer() { delete[] data; } 
};

