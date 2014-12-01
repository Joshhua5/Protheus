/*************************************************************************
Protheus Source File.
Copyright (C), Protheus Studios, 2013-2014.
-------------------------------------------------------------------------

Description:
A extern class to provide reading functions to a buffer

Note: Possile optimisaton, replace memcpy with
buffer casts
-------------------------------------------------------------------------
History:
- 28:06:2014: Waring J.
*************************************************************************/
#pragma once

#include "BufferIO.h"
#include "classDefinition.h"

namespace Pro {
	namespace Util {
		// All reads and offsets are in bytes
		extern class BufferReader :
			public BufferIO
		{
		public:
			BufferReader(CBuffer* buffer);
			~BufferReader();

			// returns a pointer to the internal buffer
			// doesn't skip memory once read
			char* read_raw() const;

			// returns a CBuffer to a copy of data
			CBuffer read(const unsigned size);

			// reads until the deliminator is found
			CBuffer read_delim(const char deliminator);

			// reads until a null terminator is found '\0'
			CBuffer read_string();

			template<typename T>
			inline T read();

			template<typename T>
			inline T* read_array(const unsigned size);

			// does not change the offset
			long read_bits(const unsigned bits);

			template<typename T>
			T serialized_read(Serializer::classDefinition def);

		};

		template<typename T>
		inline T BufferReader::read() {
			return *static_cast<T*>(read(sizeof(T)).data());
		}

		// Possible memory leak, must delete returned value
		template<typename T>
		inline T* BufferReader::read_array(const unsigned size) {  
			auto buffer = read(sizeof(T) * size);
			auto out = buffer.data();
			buffer.dereference(); 
			return static_cast<T*>(out);
		}

		template<typename T>
		T BufferReader::serialized_read(Serializer::classDefinition def) {
			T out;

			vector<Member> loaded_members;

			const auto member_count = read<unsigned short>();

			// Check if there's a missmatch of extern classDefinitons
			if (member_count != def.getMembers().size()) {
				string err = "";
				for each(const auto members in def.getMembers())
					err += members.name + "\n";
				error.reportError("Missmatch of extern class definition" + err);
				return;
			}


			// Load the extern class from the buffer
			for (auto x = member_count; x != 0; --x) {
				Serializer::Member m;

				m.name = string(read_array<char>(32));
				m.size = read<unsigned>();
				m.data = read(m.size);

				loaded_members.push_back(m);
			}

			// check for a match between the definition
			// and what's been loaded
			// upon a match, load the data into the object
			for each(const auto& member in def.getMembers())
				for each(const auto& loaded_member in loaded_members) {
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
	}
}
