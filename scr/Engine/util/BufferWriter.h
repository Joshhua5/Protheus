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
			void write(void* value, unsigned size);
			  
			template<typename T>
			void inline write(T data){ 
				write(&data, sizeof(T));
			}

			template<typename T>
			void inline write_array(T* data, unsigned size){
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

			static int lWriteBuffer(lua_State*);
			static int lWriteString(lua_State*);
			static int lWriteUInt(lua_State*);
			static int lWriteInt(lua_State*);
			static int lWriteDouble(lua_State*);
			static int lCreate(lua_State*);
			static string lGetMetatable(){
				return "buffer_writer_metatable";
			}

			template<typename T>
			static inline void lGetFunctions(std::vector<luaL_Reg>& fields){
				BufferIO::lGetFunctions<T>(fields);

				fields.push_back({ "writeBuffer", &T::lWriteBuffer });
				fields.push_back({ "writeString", &T::lWriteString });
				fields.push_back({ "writeUInt", &T::lWriteUInt });
				fields.push_back({ "writeInt", &T::lWriteInt });
				fields.push_back({ "writeDouble", &T::lWriteDouble });
			}
		};
	}
}
