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

#include "classDefinition.h"

namespace Pro{ 
		using namespace Serializer;

		class BufferWriter :
			public BufferIO
		{
		public:
			BufferWriter(CBuffer* buffer);
			~BufferWriter();

			// writes a value to the buffer at the writer,
			// of the size specified
			void write(const void* value,const unsigned size);
			  
			template<typename T>
			void inline write(const T& data){ 
				write(&data, sizeof(T));
			}

			template<typename T>
			void inline write(const T&& data){ 
				write(&data, sizeof(T));
			}

			// Writes the array into the buffer, size in bytes
			template<typename T>
			void inline write_array(T* data, unsigned size){
				write((void*)data, size);
			}

			// writes the amount of elements in an array
			template<typename T>
			void inline write_elements(T* data, unsigned elements){
				write((void*) data, elements * sizeof(T));
			}

			template<typename T>
			void serialized_write(classDefinition def, T* data){
				const auto members = def.getMembers();

				// Write the amount of members in the extern class
				write<unsigned short>(members.size());

				// Write each member
				for each(const auto member in members){
					write_array<char>(member.name, 32);

					// Pointer to the data member we want
					const auto member_pointer =
						static_cast<char*>(data)+
						member.offset;

					// Write the size of the member
					write<unsigned>(member.size);
					// Write the data of the member
					write(member_pointer, member.size);
				}
			}
			 
		}; 

		BufferWriter::BufferWriter(CBuffer* _buffer) {
			m_buffer = _buffer;
			m_head = 0;
		}

		BufferWriter::~BufferWriter()
		{
			m_buffer = nullptr;
		}

		void BufferWriter::write(const void* value, const unsigned size) {
			if (m_buffer == nullptr)
				return;

			// Check if the write will overflow
			if (m_head + size > m_buffer->size())
				// Resizes the buffer with 64 bytes overhead
				m_buffer->resize(m_head + size + 64);

			memcpy(m_buffer->at(m_head), value, size);
			skip(size);
		}

}
