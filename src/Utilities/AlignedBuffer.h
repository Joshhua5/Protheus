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

namespace Pro {

	class AlignedBuffer :
		public BufferBase
	{
	protected:
		unsigned char m_offset;
		unsigned char m_padding;
		unsigned char m_alignment_crossover;
		unsigned short m_sizeof;
		unsigned m_alignment;
	public:
		// Aligns the internal structure to the byte bountry passed,
		// The data in the buffer must be a multiple and smaller
		// than the boundary, the copy is set to false but the data isn't correctly
		// aligned then a copy will be performed anyway.
		// The data type in the buffer must be a multiple and smaller  than the boundary
		// if Padding is applied then only the maximum amount of instances of the 
		// data will be contained in a cache line, if data is spread over two cachelines
		// then the data will be padded to only use one line, this uses more memory
		// size of object must be provided
		// If passing in predefined data, the size variable now defined the size of the prexisting data in byte
		// data will be copied
		AlignedBuffer(const unsigned size, const unsigned sizeOf, void* data = nullptr, const unsigned alignment = 64);

		~AlignedBuffer();

		// interate through the boundaries
		// equivilant to 
		// this->data()[pos * alignment];
		void* atBoundary(unsigned pos) const;

		// returns the data with bounds checking
		void* at(unsigned pos) const;

		// returns the defined alignement for the data
		unsigned alignment() const;

		// returns a pointer to the internal data
		void* data();

		// size of the buffer in bytes
		unsigned size() const;

		// access data in the buffer without
		// boundary checking
		void* operator[](unsigned pos);

		// Dereferencing does nothing with an
		// aligned buffer
		void dereference() = delete;
	};
}