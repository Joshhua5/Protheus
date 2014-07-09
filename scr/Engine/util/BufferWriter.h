/*************************************************************************
Protheus Source File.
Copyright (C), Protheus Studios, 2013-2014.
-------------------------------------------------------------------------

Description:
A class to provide writing functions to a buffer

-------------------------------------------------------------------------
History:
- 1:06:2014: Waring J.
*************************************************************************/
#pragma once

#include "BufferIO.h"

#include "ClassDefinition.h"

namespace Pro{
	namespace Util{
		using namespace Serializer;

		class BufferWriter :
			public BufferIO
		{
		public:
			BufferWriter(CBuffer* buffer);
			~BufferWriter();

			// writes a value to the buffer at the writer,
			// of the size specified
			void write(void* value, unsigned int size);

			template<typename T>
			void inline write(T){
				T data;
				write(&data, sizeof(T));
			}

			template<typename T>
			void inline write_array(T* data, unsigned int size){
				write((void*)data, size);
			}

			template<typename T>
			void serialized_write(ClassDefinition def, T* data){
				const auto members = def.getMembers();

				// Write the amount of members in the class
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
	}
}
