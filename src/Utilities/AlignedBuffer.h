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
		AlignedBuffer(const unsigned size, const unsigned sizeOf, void* data = nullptr, const unsigned alignment = 64) {

			// Check sizeof

			if (sizeOf == 0 || size < sizeOf)
				return;

			// Calculate padding

			m_alignment = alignment;
			m_sizeof = sizeOf;
			m_alignment_crossover = static_cast<unsigned char>(floor(m_sizeof / m_alignment)) + 1;
			m_padding = (m_alignment * m_alignment_crossover) % m_sizeof;

			// initialize data for alignment

			m_data = new char[size + ((size / m_alignment / m_alignment_crossover) * m_padding) + m_alignment];
			long i_data = (long)m_data;
			m_offset = static_cast<unsigned char>(m_alignment - (i_data % m_alignment));
			m_data = static_cast<char*>(m_data) + m_offset;

			// Copy the data into the new buffer,
			// adding in padding
			if (data != nullptr) {
				if (m_padding == 0)
					memcpy(m_data, data, size);
				else {
					char* m_read = (char*)data;
					char* m_writer = (char*)m_data;
					for (unsigned x = 0; x < size / m_sizeof; ++x) {
						memcpy(m_writer, m_read, m_sizeof);
						m_writer += m_sizeof + m_padding;
						m_read += m_sizeof;
					}
				}
			}
		}

		~AlignedBuffer() {
			m_data = static_cast<char*>(m_data) - m_offset;
			delete[] m_data;
		}

		// interate through the boundaries
		// equivilant to 
		// this->data()[pos * alignment];
		void* atBoundary(unsigned pos) const {
			return static_cast<char*>(m_data) + (pos * m_alignment_crossover * m_alignment);
		}

		// returns the data with bounds checking
		void* at(unsigned pos) const {
			const unsigned offset = pos * m_sizeof;
			const unsigned position = (offset + ((offset / m_alignment) * (m_padding / m_alignment_crossover)));
			if (pos >= position)
				return nullptr;
			return static_cast<char*>(m_data) + position;
		};

		// returns the defined alignement for the data
		unsigned alignment() const {
			return m_alignment;
		}

		// returns a pointer to the internal data
		void* data() {
			return m_data;
		}

		// size of the buffer in bytes
		unsigned size() const {
			return m_size;
		}

		// access data in the buffer without
		// boundary checking
		void* operator[](unsigned pos) {
			const unsigned offset = pos * m_sizeof;
			const unsigned position = (offset + ((offset / m_alignment) * (m_padding / m_alignment_crossover)));
			return static_cast<char*>(m_data) + position;
		}

		// Dereferencing does nothing with an
		// aligned buffer
		void dereference() = delete;
	};

}