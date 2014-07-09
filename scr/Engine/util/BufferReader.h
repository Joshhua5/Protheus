/*************************************************************************
Protheus Source File.
Copyright (C), Protheus Studios, 2013-2014.
-------------------------------------------------------------------------

Description:
A class to provide reading functions to a buffer

Note: Possile optimisaton, replace memcpy with
buffer casts
-------------------------------------------------------------------------
History:
- 28:06:2014: Waring J.
*************************************************************************/
#pragma once

#include "BufferIO.h"
#include "ClassDefinition.h"

namespace Pro{
	namespace Util{
		class BufferReader :
			public BufferIO
		{
		public:
			BufferReader(CBuffer* buffer);
			~BufferReader();

			// returns a pointer to the internal buffer
			// doesn't skip memory once read
			char* read_raw();
			// returns a char* to a copy of data
			char* read(int size);

			template<typename T>
			inline T read(){
				return *(T*)read(sizeof(T));
			}

			template<typename T>
			inline T* read_array(unsigned int size){
				return (T*)read(sizeof(T) * size);
			}

			template<typename T>
			T serialized_read(Serializer::ClassDefinition def) {
				T out;

				vector<Member> loaded_members;

				const auto member_count = read<unsigned short>();

				// Check if there's a missmatch of ClassDefinitons
				if (member_count != def.getMembers().size()){
					throw ""
				}
				return;

				// Load the class from the buffer
				for (auto x = member_count; x != 0; --x){
					Serializer::Member m;

					m.name = string(read_array<char>(32));
					m.size = read<unsigned>();
					m.data = read(m.size);

					loaded_members.push_back(m);
				}

				// check for a match between the definition
				// and what's been loaded
				// upon a match, load the data into the object
				for each(const auto member in def.getMembers())
					for each(const auto loaded_member in loaded_members){
					if (member.name != loaded_member.name)
						continue;

					// Get the pointer to the member
					const auto member_pointer =
						static_cast<char*>(&out) +
						member.offset;

					// copy the data into the object
					memcpy(member_pointer,
						loaded_member.data,
						loaded_member.size);
					}
				return out;
			}
		};
	}
}
