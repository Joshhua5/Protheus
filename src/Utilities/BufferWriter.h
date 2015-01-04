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
			unsigned char m_reoccurring_resize;
			std::atomic<bool> being_resized;

		public:
			BufferWriter(Buffer* buffer) {
				using_smart = false;
				being_resized = false;
				m_reoccurring_resize = 0;
				m_buffer = buffer;
				m_head = 0;
			}
			BufferWriter(smart_pointer<Buffer> pointer) {
				using_smart = true;
				being_resized = false;
				m_reoccurring_resize = 0;
				m_buffer = pointer;
				m_head = 0;
			}
			~BufferWriter() {
				m_head = 0;
				if (using_smart)
					m_buffer = nullptr;
				else
					m_buffer.dereference();
			}

			/*!	Writes an array into the buffer
				Size is in bytes
			*/
			inline void write(const void* value, const unsigned size) {
				if (m_buffer == nullptr)
					return;
				skip(size);  
				auto l_head = m_head - size;

				// Check if the write will overflow
				if (m_head > m_buffer->size() && !being_resized)
					// Resizes the buffer exponentially as more resizes are called
					being_resized = true;
					m_buffer->resize(static_cast<unsigned>((float)(m_head) * (1.f + (m_reoccurring_resize++ / 10.f))));
					being_resized = false;

				m_buffer->lk.lock(); 
				memcpy(m_buffer->at(l_head), value, size);
				m_buffer->lk.unlock();
			}

			inline void write_nl(const void* value, const unsigned size) {
				if (m_buffer == nullptr)
					return;
				skip(size);
				auto l_head = m_head - size;

				// Check if the write will overflow
				if (l_head > m_buffer->size() && !being_resized){
					// Resizes the buffer exponentially as more resizes are called
					being_resized = true;
					m_buffer->resize(static_cast<unsigned>((float) (l_head + size) * (1.f + (m_reoccurring_resize++ / 10.f))));
					being_resized = false;
				} 
				memcpy(m_buffer->at(l_head), value, size);
			}

			template<typename T>
			inline void write(const T& data) {
				if (being_resized)
					write(&data, sizeof(T));
				else
					write_nl(&data, sizeof(T));
			}
			  
			/*! Writes an object into the buffer */
			template<typename T>
			inline void write(const T&& data) {
				if (being_resized)
					write(&data, sizeof(T));
				else
					write_nl(&data, sizeof(T));
			}
			 
			/*! Writes the array into the buffer
				size in bytes
			*/
			template<typename T>
			inline void write_array(const T* data, unsigned size) { 
				if (being_resized)
					write(data, size);
				else
					write_nl(data, size);
			}

			/*! Writes the array into the buffer
				size in array size
			*/
			template<typename T>
			inline void write_elements(const T* data, unsigned elements) { 
				if (being_resized)
					write((void*) data, elements * sizeof(T));
				else
					write_nl((void*) data, elements * sizeof(T));
			}

			template<typename T, size_t size>
			inline void write_elements(T (&array)[size]) {
				write(&array, size * sizeof(T));
				if (being_resized)
					write(&array, size * sizeof(T));
				else
					write_nl(&array, size * sizeof(T));
			} 

			/*! Writes a complex data type from the buffer according to the class definition */
			template<typename T>
			inline void serialized_write(const ClassDefinition& def, const T* data) {

				const auto members = def.getMembers();

				m_buffer->lk.lock();
				// Write the amount of members in the extern class
				write<unsigned short>(members.size());

				// Write each member
				for each(const auto member in members) {
					write_array<char>(member.name, 32);

					// Pointer to the data member we want
					const auto member_pointer =
						static_cast<char*>(data) +
						member.offset;

					// Write the size of the member
					write<unsigned>(member.size);
					// Write the data of the member
					write(member_pointer, member.size);
				} 
				m_buffer->lk.unlock();
			}
		};
	}
}