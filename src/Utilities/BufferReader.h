/*************************************************************************
Protheus Source File.
Copyright (C), Protheus Studios, 2013-2016.
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
		/*! Class to add on reading functionality to a Buffer and AlignedBuffer(Planned) */
		class BufferReader :
			public BufferIO
		{
		public:
			BufferReader(const BufferReader&) = delete;

			BufferReader(const Buffer* buffer) {
				head_ = 0;
                buffer_ = (Buffer*)buffer;
			}
            
			BufferReader(BufferReader&& buffer) {
				buffer_ = buffer.buffer_;
				head_ = buffer.head_;
				buffer.buffer_ = nullptr;
			}
            
			~BufferReader() {
				head_ = 0;
			}

			/*! Returns a pointer to the internal buffer
				doesn't skip memory once read. */
			const char* ReadRaw() const {
				return buffer_->data<char>() + head_;
			}

			char* ReadRaw() {
				return buffer_->data<char>() + head_;
			}

			/*!
				Returns a Buffercontaining the read data,
				if copy is false then the Bufferpoints to the
				existing data in @m_buffer.
			*/
			Buffer Read(const unsigned size, bool copy = true) {
				if (head_ - buffer_->size() <= size)
					return Buffer(0);
				Buffer out(ReadRaw(), size, copy);
				Skip(size);
				return std::move(out);
			}

			/*! Reads through the buffer until the next deliminator is found.
				Includes the found deliminator the deliminator.
				If copy is false then the Bufferpoints at the internal buffer.
			*/
			Buffer ReadDelim(const char deliminator, bool copy = true) {
				return Read(Find(deliminator), copy);
			}

			//! Reads until a null terminator is found '\0'
			std::string ReadString() {
				return  std::string(ReadDelim('\0').data<char>());
			}

			/*! Reads the size and appends a null terminator*/
			std::string ReadString(const unsigned size) {
				char* arr = ReadArray<char>(size + 1);
				Skip(-1);
				arr[size] = '\0';
				std::string out(arr);
				delete[] arr;
				return  std::move(out);
			}

			/*! False if @m_head is at the end of the buffer. */
			bool HasNext() const {
				return head_ < buffer_->size();
			}

			/*!
				Scans the buffer for the array of data and returns it's position
				Returns -1 if a match can't be found
			*/
			template<typename T>
			int Contains(const T* _data, unsigned size = 1) const {
				// Create a local copy 
				auto buffer = buffer_->data<T>();
				// Scan over the buffer for the combination
				for (unsigned head = head_; head < buffer_->size() - size; ++head) {
					// Check that position for the combination
					for (unsigned x = 0; *(buffer + head + x) == *(_data + x); ++x)
						if (x == size - 1)
							return head - head_;
				}
				return -1;
			}

			/*!
				Reads the next T in the buffer
				If copy is false then the Bufferpoints at the internal buffer.
			*/
			template<typename T>
			inline T Read(bool copy = true) {
				return *static_cast<T*>(Read(sizeof(T), copy).data());
			}

			/*!
				Reads the an array of T in the buffer
				If copy is false then the Bufferpoints at the internal buffer.
			*/
			template<typename T>
			inline T* ReadArray(const unsigned size, bool copy = true) {
				auto buffer = Read(sizeof(T) * size, copy);
				auto out = buffer.data();
				buffer.Dereference();
				return static_cast<T*>(out);
			}

			/*!
				Reads a specific amount of bits at the @m_head
				Does not move the @m_head
			*/
			uint64_t ReadBits(const unsigned bits) const {
				// TEST
				if (bits < 8)
					return (char)*ReadRaw() & static_cast<unsigned>((pow(2, bits) - 1));
				else if (bits < 16)
					return (short)*ReadRaw() & static_cast<unsigned>((pow(2, bits) - 1));
				else if (bits < 32)
					return (int)*ReadRaw() & static_cast<unsigned>((pow(2, bits) - 1));
				else if (bits < 64)
					return (long)*ReadRaw() & static_cast<unsigned>((pow(2, bits) - 1));
				return 0;
			};

			/*! Reads a complex data type from the buffer according to the class definition */
			template<typename T>
			T SerializedRead(Serializer::ClassDefinition& def) {
				T out;

                std::vector<Serializer::Member> loaded_members;

				const auto member_count = Read<unsigned short>();

				// Check if there's a missmatch of extern classDefinitons
				if (member_count != def.members().size()) {
                    std::string err = "";
                    
                    for(auto& member : def.members())
                        err += member.name + "\n";
                    
                    global_log.Report<LogCode::FAULT>("Missmatch of extern class definition" + err, __FUNCTION__, __LINE__);
					return;
				}


				// Load the extern class from the buffer
				for (auto x = member_count; x != 0; --x) {
					Serializer::Member m;

                    m.name = std::string(ReadArray<char>(32));
					m.size = Read<unsigned>();
                    Buffer temp_data = Read(m.size);
					m.data = temp_data.data();
                    temp_data.Dereference();

					loaded_members.push_back(m);
				}

				// check for a match between the definition
				// and what's been loaded
				// upon a match, load the data into the object
                for(const auto& member : def.members())
                    for(const auto& loaded_member : loaded_members) {
						if (member.name != loaded_member.name)
							continue;

						// Get the pointer to the member
						const auto member_pointer =
							static_cast<char*>(&out) +
							member.offset;

						// copy the data into the object
						std::memcpy(member_pointer,
							loaded_member.data,
							loaded_member.size);
					}
				return out;
			}
		};
	}
}