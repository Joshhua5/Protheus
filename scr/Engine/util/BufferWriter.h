/*************************************************************************
Protheus Source File.
Copyright (C), Protheus Studios, 2013-2014.
-------------------------------------------------------------------------

Description:
	A class to provideo writing functions to a buffer

-------------------------------------------------------------------------
History:
- 1:06:2014: Waring J.
*************************************************************************/
#pragma once


#include "CBuffer.h"


class CBufferWriter
{
	unsigned int writerPosition;
	CBuffer* buffer;
public:
	CBufferWriter(CBuffer* buffer);
	CBufferWriter(CBuffer& buffer);
	~CBufferWriter();

	// writes a value to the buffer at the writer,
	// of the size specified
	void write(void* value, int size);
	// sets the position of the writer
	void setPosition(unsigned int);
	// returns the position of the writer
	unsigned int getPosition();
	// skips over the current writing position
	void skip(int);
};

