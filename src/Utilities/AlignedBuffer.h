/*************************************************************************
Protheus Source File.
Copyright (C), Protheus Studios, 2013-2014.
-------------------------------------------------------------------------

Description:
	

-------------------------------------------------------------------------
History:
- 13:07:2014: Waring J.
*************************************************************************/
#pragma once

#include "BufferBase.h"

extern class AlignedBuffer :
	public BufferBase
{
protected:
	unsigned m_alignment;
	int m_offset;
public:
	// Aligns the internal structure to the byte bountry passed,
	// The data in the buffer must be a multiple and smaller
	// than the boundary, the copy is set to false but the data isn't correctly
	// aligned then a copy will be performed anyway.
	AlignedBuffer(void* data, const unsigned size,const int alignment,const bool copy);
	// Aligns the internal structure to the byte bountry passed,
	// The data in the buffer must be a multiple and smaller
	// than the boundary
	// if Padding is true then only the maximum amount of instances of the 
	// data will be contained in a cahche line, if data is spread over two cachelines
	// then the data will be padded to only use one line, this uses more memory
	// size of object must be provided
	AlignedBuffer(const unsigned size, unsigned alignment, const bool padding, const unsigned sizeOf);
	// Padding is off by default
	AlignedBuffer(const unsigned size, unsigned alignment);
	// Default alignment of 64 Bytes
	// to match a cache line
	AlignedBuffer(const unsigned size);
	~AlignedBuffer();

	// interate through the boundaries
	// equivilant to 
	// this->data()[pos * alignment];
	void atBoundary(unsigned pos);

	// returns the data with bounds checking
	void* at(unsigned pos);

	// returns the defined alignement for the data
	unsigned alignment();

	// returns a pointer to the internal data
	void* data();

	// size of the buffer in bytes
	unsigned size();

	// access data in the buffer without
	// boundary checking
	void* operator[](unsigned pos);

	// Dereferencing does nothing with an
	// aligned buffer
	void dereference(){}
};