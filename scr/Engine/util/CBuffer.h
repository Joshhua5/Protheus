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
	// if copy is true then the data is copied otherwise
	// the data is moved, you can't delete the pointer
	// which was used to pass data otherwise you'll
	// delete the internal data aswell.
	CBuffer(void* _data, unsigned int _size, bool copy){
		if (copy)
			memcpy(data, _data, _size);
		else
			data = _data;

		size = _size;
	}

	CBuffer(unsigned int _size)
	{
		size = _size;
		data = new char[size];
	}

	CBuffer()
	{
		size = 0;
		data = nullptr;
	}

	~CBuffer()
	{
		delete[] data;
	}

	void init(void* _data, unsigned int _size, bool copy){
		// Check if data has been initialized
		if (data != nullptr)
			delete[] data;

		if (copy){
			data = new char[_size];
			memcpy(data, _data, _size);
		}
		else
			data = _data;

		size = _size;
	}

	void init(unsigned int _size){
		init(new char[_size], _size, false);
	}

	// removes the internet reference
	// to prevent the data from being deleted
	// when CBuffer is deleted.
	// don't dereference unless the data pointer
	// has been stored somewhere else (memory leak)
	void dereference(){
		data = nullptr;
		size = 0;
	}
};
