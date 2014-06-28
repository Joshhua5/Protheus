/*************************************************************************
Protheus Source File.
Copyright (C), Protheus Studios, 2013-2014.
-------------------------------------------------------------------------

Description:
	A class to provide reading functions to a buffer

	Note: Possile optimisaton, replace memcpy with 
	buffer casts
-------------------------------------------------------------------------
History:
- 28:06:2014: Waring J.
*************************************************************************/
#pragma once

#include "BufferIO.h"

class BufferReader :
	public BufferIO
{
public: 
	BufferReader(CBuffer* buffer); 
	~BufferReader();

	// returns a pointer to the internal buffer
	// doesn't skip memory once read
	char* read_raw();
	// returns a char* to a copy of data
	char* read(int size);
 
	template<typename T>
	inline T read(){
		return *(T*) read(sizeof(T));
	}
};

