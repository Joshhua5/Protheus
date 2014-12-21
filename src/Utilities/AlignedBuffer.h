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
	namespace Util {

		/*!
			AlignedBuffer will create a buffer with an alignment in bytes on the heap.
			AlignedBuffer::data() % alignment, will always equal 0

			Purpose is to allow for optimised access to data by being more cache friendly,
			also works well with SSE with an alignment of 16 bytes.

			Copy and move constructors must be utilised to handle pointer offsets when parsing
		*/
		class AlignedBuffer :
			public BufferBase
		{
		protected:
			unsigned char m_offset;

			/*! How much data in bytes is added per instance to keep valid alignment */
			unsigned char m_padding;

			/*! The amount of times a single instance of data crosses over the alignment size
				EG. an alignment of 2 storing data of size 4 will cross over 2 alignment barriers.
			*/
			unsigned char m_alignment_crossover;

			/*! Size of the instances being stored in bytes*/
			unsigned short m_sizeof;
			unsigned m_alignment;
		public:
			/*! Non copyable */
			AlignedBuffer(const AlignedBuffer&) = delete;
			/*! Non copyable */
			AlignedBuffer& operator=(const AlignedBuffer&) = delete;

			/*!
				Arguments:
				size - size of data to reserve in bytes
				sizeOf - Size of the object which is going to be stored (used to process alignment data)
				data (default nullptr) - pointer to a struture to be copied into the alignment buffer.
				alignment (default 64) - The alignment of memory in bytes (data % alignmnet will equal 0)

				Note:
					-The size of the @data must be larger than or equal to the @size
					-All sizes are calculated in bytes
			*/
			AlignedBuffer(const unsigned size, const unsigned sizeOf, void* data = nullptr, const unsigned alignment = 64) {

				// Check sizeof

				if (sizeOf == 0 || size < sizeOf)
					return;

				// Calculate padding and alignment_crossover

				m_alignment = alignment;
				m_sizeof = sizeOf;
				m_alignment_crossover = static_cast<unsigned char>(floor(m_sizeof / m_alignment)) + 1;
				m_padding = (m_alignment * m_alignment_crossover) % m_sizeof;

				// initialize data for alignment

				m_data = new char[size + ((size / m_alignment / m_alignment_crossover) * m_padding) + m_alignment];
				long i_data = (long)m_data;
				m_offset = static_cast<unsigned char>(m_alignment - (i_data % m_alignment));
				m_data = static_cast<char*>(m_data) + m_offset;

				// Copy the data into the new buffer, adding in padding
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
				// recrease the m_data so that delete[] will get the correct pointer
				m_data = static_cast<char*>(m_data) - m_offset;
				delete[] m_data;
			}

			AlignedBuffer(AlignedBuffer&& rhs) {
				m_offset = rhs.m_offset;
				m_padding = rhs.m_padding;
				m_alignment_crossover = rhs.m_alignment_crossover;
				m_alignment = rhs.m_alignment;
				m_data = rhs.m_data;
				m_size = rhs.m_size;
				m_sizeof = rhs.m_sizeof;
				// dereference in the move
				rhs.m_data = nullptr;
			}

			inline AlignedBuffer& operator=(AlignedBuffer&& rhs) {
				m_offset = rhs.m_offset;
				m_padding = rhs.m_padding;
				m_alignment_crossover = rhs.m_alignment_crossover;
				m_alignment = rhs.m_alignment;
				m_data = rhs.m_data;
				m_size = rhs.m_size;
				m_sizeof = rhs.m_sizeof;
				// dereference in the move
				rhs.m_data = nullptr;
			}

			/*!
				Returns the data at a specific alignment boundary
				equivilant to data()[pos * m_alignment_crossover * m_alignment];
			*/
			void* atBoundary(unsigned pos) const {
				return static_cast<char*>(m_data) + (pos * m_alignment_crossover * m_alignment);
			}

			/*! Returns the data at the index with bounds checking */
			void* at(unsigned index) const {
				const unsigned offset = index * m_sizeof;
				const unsigned position = (offset + ((offset / m_alignment) * (m_padding / m_alignment_crossover)));
				if (index >= position)
					return nullptr;
				return static_cast<char*>(m_data) + position;
			};

			/*! Returns the defined alignement for the data */
			unsigned alignment() const {
				return m_alignment;
			}

			/*! Returns a pointer to the internal data with offset
				Not safe to call delete on
			*/
			void* data() {
				return m_data;
			}

			/*! Size of the buffer in bytes excluding offset */
			unsigned size() const {
				return m_size;
			}

			/*! Returns the data at the index */
			void* operator[](unsigned index) {
				const unsigned offset = index * m_sizeof;
				const unsigned position = (offset + ((offset / m_alignment) * (m_padding / m_alignment_crossover)));
				return static_cast<char*>(m_data) + position;
			}
		};
	}
}