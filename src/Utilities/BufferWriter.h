/*************************************************************************
Protheus Source File.
Copyright (C), Protheus Studios, 2013-2014.
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
		/*! Class to add on writing functionality to a Bufferand AlignedBuffer(Planned) */
		class BufferWriter :
			public BufferIO
		{
			unsigned char reoccurring_resize_;
			std::atomic<bool> being_resized_;

		public:
			BufferWriter(Buffer* buffer) {
				using_smart = false;
				being_resized_ = false;
				reoccurring_resize_ = 0;
				buffer_ = buffer;
				head_ = 0;
			}
			BufferWriter(smart_ptr<Buffer> pointer) {
				using_smart = true;
				being_resized_ = false;
				reoccurring_resize_ = 0;
				buffer_ = pointer;
				head_ = 0;
			}
			~BufferWriter() {
				head_ = 0;
				if (using_smart)
					buffer_ = nullptr;
				else
					buffer_.dereference();
			}

			/*!	Writes an array into the buffer
				Size is in bytes
			*/
			inline void Write(const void* value, const unsigned size) {
				if (buffer_ == nullptr)
					return;
				Skip(size);  
				auto l_head = head_ - size;

				// Check if the write will overflow
				if (head_ > buffer_->size() && !being_resized_)
					// Resizes the buffer exponentially as more resizes are called
					being_resized_ = true;
					buffer_->Resize(static_cast<unsigned>((float)(head_) * (1.f + (reoccurring_resize_++ / 10.f))));
					being_resized_ = false;

				buffer_->lock(); 
				memcpy(buffer_->At(l_head), value, size);
				buffer_->unlock();
			}

			inline void WriteNoLock(const void* value, const unsigned size) {
				if (buffer_ == nullptr)
					return;
				Skip(size);
				auto l_head = head_ - size;

				// Check if the write will overflow
				if (l_head > buffer_->size() && !being_resized_){
					// Resizes the buffer exponentially as more resizes are called
					being_resized_ = true;
					buffer_->Resize(static_cast<unsigned>((float) (l_head + size) * (1.f + (reoccurring_resize_++ / 10.f))));
					being_resized_ = false;
				} 
				memcpy(buffer_->At(l_head), value, size);
			}

			template<typename T>
			inline void Write(const T& data) {
				if (being_resized_)
					Write(&data, sizeof(T));
				else
					WriteNoLock(&data, sizeof(T));
			}
			  
			/*! Writes an object into the buffer */
			template<typename T>
			inline void Write(const T&& data) {
				if (being_resized_)
					Write(&data, sizeof(T));
				else
					WriteNoLock(&data, sizeof(T));
			}
			 
			/*! Writes the array into the buffer
				size in bytes
			*/
			template<typename T>
			inline void WriteArray(const T* data, unsigned size) { 
				if (being_resized_)
					Write(data, size);
				else
					WriteNoLock(data, size);
			}

			/*! Writes the array into the buffer
				size in array size
			*/
			template<typename T>
			inline void WriteElements(const T* data, unsigned elements) { 
				if (being_resized_)
					Write((void*) data, elements * sizeof(T));
				else
					WriteNoLock((void*) data, elements * sizeof(T));
			}

			template<typename T, size_t size>
			inline void WriteElements(T (&array)[size]) {
				Write(&array, size * sizeof(T));
				if (being_resized_)
					Write(&array, size * sizeof(T));
				else
					WriteNoLock(&array, size * sizeof(T));
			} 

			/*! Writes a complex data type from the buffer according to the class definition */
			template<typename T>
			inline void SerializedWrite(const ClassDefinition& def, const T* data) {

				const auto members = def.members();

				buffer_->lock();
				// Write the amount of members in the extern class
				Write<unsigned short>(members.size());

				// Write each member
				for each(const auto member in members) {
					WriteArray<char>(member.name, 32);

					// Pointer to the data member we want
					const auto member_pointer =
						static_cast<char*>(data) +
						member.offset;

					// Write the size of the member
					Write<unsigned>(member.size);
					// Write the data of the member
					Write(member_pointer, member.size);
				} 
				buffer_->unlock();
			}
		};
	}
}