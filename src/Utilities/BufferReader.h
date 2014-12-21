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
#include "ClassDefinition.h"

namespace Pro {
	namespace Util {
		/*! Class to add on reading functionality to a Bufferand AlignedBuffer(Planned) */
		class BufferReader :
			public BufferIO
		{
		public:
			BufferReader(const BufferReader&) = delete;

			BufferReader(Buffer* buffer) {
				using_smart = false;
				m_head = 0;
				m_buffer = buffer;
			}
			BufferReader(smart_pointer<Buffer> pointer) {
				using_smart = true;
				m_head = 0;
				m_buffer = pointer;
			}
			BufferReader(BufferReader&& buffer) {
				m_buffer = buffer.m_buffer;
				m_head = buffer.m_head;
				buffer.m_buffer = nullptr;
			}
			~BufferReader() {
				m_head = 0;
				if (using_smart)
					m_buffer = nullptr;
				else
					m_buffer.dereference();
			}

			/*! Returns a pointer to the internal buffer
				doesn't skip memory once read. */
			char* read_raw() const {
				return m_buffer->data<char>() + m_head;
			}

			/*!
				Returns a Buffercontaining the read data,
				if copy is false then the Bufferpoints to the
				existing data in @m_buffer.
			*/
			Buffer read(const unsigned size, bool copy = true) {
				if (m_head - m_buffer->size() <= size)
					return Buffer(0);
				Buffer out(read_raw(), size, copy);
				skip(size);
				return move(out);
			}

			/*! Reads through the buffer until the next deliminator is found.
				Includes the found deliminator the deliminator.
				If copy is false then the Bufferpoints at the internal buffer.
			*/
			Buffer read_delim(const char deliminator, bool copy = true) {
				return read(find(deliminator), copy);
			}

			//! Reads until a null terminator is found '\0'
			string read_string() {
				return string(read_delim('\0').data<char>());
			}

			/*! Reads the size and appends a null terminator*/
			string read_string(const unsigned size) {
				char* arr = read_array<char>(size + 1);
				skip(-1);
				arr[size] = '\0';
				string out(arr);
				delete[] arr;
				return move(out);
			}

			/*! False if @m_head is at the end of the buffer. */
			bool hasNext() const {
				return m_head < m_buffer->size();
			}

			/*!
				Scans the buffer for the array of data and returns it's position
				Returns -1 if a match can't be found
			*/
			template<typename T>
			int contains(const T* data, unsigned size = 1) const {
				// Create a local copy 
				T* buffer = m_buffer->data<T>();
				// Scan over the buffer for the combination
				for (unsigned head = m_head; head < m_buffer->size() - size; ++head) {
					// Check that position for the combination
					for (unsigned x = 0; *(buffer + head + x) == *(data + x); ++x)
						if (x == size - 1)
							return head - m_head;
				}
				return -1;
			}

			/*!
				Reads the next T in the buffer
				If copy is false then the Bufferpoints at the internal buffer.
			*/ 
			template<typename T>
			inline T read(bool copy = true) {
				return *static_cast<T*>(read(sizeof(T), copy).data());
			}

			/*!
				Reads the an array of T in the buffer
				If copy is false then the Bufferpoints at the internal buffer.
			*/ 
			template<typename T>
			inline T* read_array(const unsigned size, bool copy = true) {
				auto buffer = read(sizeof(T) * size, copy);
				auto out = buffer.data();
				buffer.dereference();
				return static_cast<T*>(out);
			}

			/*!
				Reads a specific amount of bits at the @m_head
				Does not move the @m_head
			*/
			long read_bits(const unsigned bits) const {
				// TEST
				if (bits < 8)
					return (char)*read_raw() & static_cast<unsigned>((pow(2, bits) - 1));
				else if (bits < 16)
					return (short)*read_raw() & static_cast<unsigned>((pow(2, bits) - 1));
				else if (bits < 32)
					return (int)*read_raw() & static_cast<unsigned>((pow(2, bits) - 1));
				else if (bits < 64)
					return (long)*read_raw() & static_cast<unsigned>((pow(2, bits) - 1));
				return 0;
			};

			/*! Reads a complex data type from the buffer according to the class definition */
			template<typename T>
			T serialized_read(Serializer::ClassDefinition def) {
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
		};
	}
}