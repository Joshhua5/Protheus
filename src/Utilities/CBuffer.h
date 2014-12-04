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

#include "BufferBase.h"  

class CBuffer :
	protected BufferBase
{
public:
	// if copy is true then the data is copied otherwise
	// the data is moved, you can't delete the pointer
	// which was used to pass data otherwise you'll
	// delete the internal data aswell.
	CBuffer(void* data, const unsigned size, const bool copy);
	CBuffer(const unsigned size); 
	CBuffer();
	// Move
	CBuffer(CBuffer&&);
	CBuffer& operator=(CBuffer&&);
	// Copy
	CBuffer(const CBuffer&);
	CBuffer& operator=(const CBuffer&);
	~CBuffer();

	

	void init(const void* data, const unsigned size, const bool copy);
	void init(const unsigned size);

	template<typename T> T* data() const{
		return reinterpret_cast<T*>(m_data);
	}

	// Counts how many times value is present in the buffer
	template<typename T> 
	unsigned count(const T& value);

	void resize(const unsigned);

	// returns void* to the buffer
	void* data() const;
	// returns size in bytes
	unsigned size() const;
	bool isEmpty() const;

	// Provides bounds checking
	void* at(const unsigned pos) const;
	void* operator[](const unsigned pos) const;
	 
	// removes the internet reference
	// to prevent the data from being deleted
	// when CBuffer is deleted.
	// don't dereference unless the data pointer
	// has been stored somewhere else (memory leak)
	void dereference(); 
};

template<typename T>
unsigned CBuffer::count(const T& value) {
	T* buffer = data<T>();
	unsigned count = 0;
	for (unsigned head = 0; head < (size() / sizeof(T)); ++head, ++buffer)
		if (*buffer == value)
			++count;
	return count;
}