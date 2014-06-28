/*************************************************************************
Protheus Source File.
Copyright (C), Protheus Studios, 2013-2014.
-------------------------------------------------------------------------

Description:
	A class to provide io base functions for buffer writer/reader

-------------------------------------------------------------------------
History:
- 1:06:2014: Waring J.
*************************************************************************/
#pragma once
#include "CBuffer.h"

class BufferIO
{
protected:
	unsigned int head;
	CBuffer* buffer;
public: 

	// sets the position of the writer
	void setPosition(unsigned int);
	// returns the position of the writer
	unsigned int getPosition();
	// skips over the current writing position
	void skip(int);
	// get's the internal buffers size
	unsigned int getBufferSize();

};
