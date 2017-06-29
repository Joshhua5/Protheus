/*************************************************************************
Protheus Source File.
Copyright (C), Protheus Studios, 2013-2016.
-------------------------------------------------------------------------

Description:
A extern class to provide writing functions to a buffer

-------------------------------------------------------------------------
History:
- 1:06:2014: Waring J.
*************************************************************************/
#pragma once

#include "BufferIO.h" 
#include "ClassDefinition.h"
#include <atomic>

namespace Pro {
	namespace Util {
		using namespace Serializer;
		class BufferWriter :
			public BufferIO
		{
			unsigned reoccurring_resize_;
		public:
            BufferWriter(Buffer* buffer)
            {
				reoccurring_resize_ = 0;
                buffer_ = buffer;
                head_ = 0;
			}
            
			~BufferWriter() {
				head_ = 0;
			}

			//!	Writes an array into the buffer
			//!	Size is in bytes
			inline void Write(const void* value, const unsigned size) {
				if (buffer_ == nullptr)
					return;
				Skip(size);
				auto l_head = head_ - size;

				// Check if the write will overflow
                if (head_ > buffer_->size()){
					// Resizes the buffer exponentially as more resizes are called
                    buffer_->Resize(static_cast<unsigned>((float)(head_) * (1.f + (reoccurring_resize_++ / 10.f))));
                }
				std::memcpy(buffer_->At(l_head), value, size);
			}
  
            template<typename T>
			inline void Write(const T& data) {
                Write(&data, sizeof(T));
			}

			/*! Writes an object into the buffer */
			template<typename T>
			inline void Write(const T&& data) {
                Write(&data, sizeof(T));
			}

			/*! Writes the array into the buffer
				size in bytes
			*/
			template<typename T>
			inline void WriteArray(const T* data, unsigned size) {
                Write(data, size);
			}

			/*! Writes the array into the buffer
				size in array size
			*/
			template<typename T>
			inline void WriteElements(const T* data, unsigned elements) {
                Write((void*)data, elements * sizeof(T));
			}

			template<typename T, size_t size>
			inline void WriteElements(T(&array)[size]) {
				Write(&array, size * sizeof(T));
			}

			/*! Writes a complex data type from the buffer according to the class definition */
			template<typename T>
			inline void SerializedWrite(const ClassDefinition& def, const T* data) {

				const auto& members_ = def.members();
 
				// Write the amount of members in the extern class
				Write<unsigned short>(members_.size());

				// Write each member
                size_t size = members_.size();
                for(unsigned x = 0; x < size; ++x){
					WriteArray<char>(members_[x].name.data(), 32);

					// Pointer to the data member we want
					const auto member_pointer =
						static_cast<char*>(data) +
						members_[x].offset;

					// Write the size of the member
					Write<unsigned>(members_[x].size);
					// Write the data of the member
					Write(member_pointer, members_[x].size);
				}
			}
		};
	}
}