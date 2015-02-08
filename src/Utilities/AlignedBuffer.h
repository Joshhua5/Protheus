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
			unsigned char offset_;

			/*! How much data in bytes is added per instance to keep valid alignment */
			unsigned char padding_;

			/*! The amount of times a single instance of data crosses over the alignment size
				EG. an alignment of 2 storing data of size 4 will cross over 2 alignment barriers.
			*/
			unsigned char alignment_crossover_;

			//! Size of the instances being stored in bytes
			unsigned short sizeof_;
			unsigned alignment_;

			//! Calculates a offset for a object's index
			inline unsigned Position(unsigned index) const { 
				const unsigned offset = index * sizeof_;
				return (offset + ((offset / alignment_) * (padding_ / alignment_crossover_)));
			}

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

				alignment_ = alignment;
				sizeof_ = sizeOf;
				alignment_crossover_ = static_cast<unsigned char>(floor(sizeof_ / alignment_)) + 1;
				padding_ = (alignment_ * alignment_crossover_) % sizeof_;

				// initialize data for alignment

				data_ = operator new(size + ((size / alignment_ / alignment_crossover_) * padding_) + alignment_);
				long i_data = (long)data_;
				offset_ = static_cast<unsigned char>(alignment_ - (i_data % alignment_));
				data_ = static_cast<char*>(data_) + offset_;

				// Copy the data into the new buffer, adding in padding
				if (data != nullptr) {
					if (padding_ == 0)
						memcpy(data_, data, size);
					else {
						char* Read = (char*)data;
						char* writer = (char*)data_;
						for (unsigned x = 0; x < size / sizeof_; ++x) {
							memcpy(writer, Read, sizeof_);
							writer += sizeof_ + padding_;
							Read += sizeof_;
						}
					}
				}
			}

			~AlignedBuffer() {
				// recrease the m_data so that delete[] will get the correct pointer
				data_ = static_cast<char*>(data_) - offset_;
				operator delete(data_);
			}

			AlignedBuffer(AlignedBuffer&& rhs) {
				offset_ = rhs.offset_;
				padding_ = rhs.padding_;
				alignment_crossover_ = rhs.alignment_crossover_;
				alignment_ = rhs.alignment_;
				data_ = rhs.data_;
				size_ = rhs.size_;
				sizeof_ = rhs.sizeof_;
				// dereference in the move
				rhs.data_ = nullptr;
			}

			inline AlignedBuffer& operator=(AlignedBuffer&& rhs) {
				offset_ = rhs.offset_;
				padding_ = rhs.padding_;
				alignment_crossover_ = rhs.alignment_crossover_;
				alignment_ = rhs.alignment_;
				data_ = rhs.data_;
				size_ = rhs.size_;
				sizeof_ = rhs.sizeof_;
				// dereference in the move
				rhs.data_ = nullptr;
			}

			/*!
				Returns the data at a specific alignment boundary
				equivilant to data()[pos * m_alignment_crossover * m_alignment];
			*/
			inline void* AtBoundary(size_t pos) const {
				return static_cast<char*>(data_) + (pos * alignment_crossover_ * alignment_);
			}

			/*! Returns the data at the index with bounds checking */
			inline void* At(size_t index) const { 
				const unsigned position = Position(index);
				if (index >= position)
					return nullptr;
				return static_cast<char*>(data_) + position;
			};

			/*! Returns the defined alignement for the data */
			inline unsigned alignment() const {
				return alignment_;
			} 

			/*! Returns the size of internal data being stored
				Value is used to calculate alignment
			*/
			inline unsigned sizeOf() const {
				return sizeof_;
			}
			  
			inline bool IsPadded() const {
				return padding_ != 0;
			} 

			/*! Returns the data at the index */
			inline void* operator[](unsigned index) { 
				return static_cast<char*>(data_) + Position(index);
			}
		};
	}
}