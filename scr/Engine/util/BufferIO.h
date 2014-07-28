/*************************************************************************
Protheus Source File.
Copyright (C), Protheus Studios, 2013-2014.
-------------------------------------------------------------------------

Description:
A class to provide io base functions for buffer writer/reader

-------------------------------------------------------------------------
History:
- 1:06:2014: Waring J.
*************************************************************************/
#pragma once
#include "CBuffer.h"

namespace Pro{
	namespace Util{
		class BufferIO
		{
		protected:
			unsigned m_head;
			CBuffer* m_buffer;
		public:

			// sets the position of the writer
			void setPosition(unsigned int);

			// returns the position of the writer
			unsigned getPosition() const;

			// skips over the current writing position
			void skip(const int);

			// get's the internal buffers size
			unsigned getBufferSize() const;

			unsigned find(const char deliminator) const;

			static int lSkip(lua_State*);
			static int lGetPosition(lua_State*);
			static int lSetPosition(lua_State*);
			static int lGetBufferSize(lua_State*);

			constexpr static const char* lGetMetatable(){
				return "buffer_io_metatable";
			}

			template<typename T>
			static inline void lGetFunctions(std::vector<luaL_Reg>& fields){
				fields.push_back({ "skip", &T::lSkip });
				fields.push_back({ "getPosition", &T::lGetPosition });
				fields.push_back({ "setPosition", &T::lSetPosition });
				fields.push_back({ "getBufferSize", &T::lGetBufferSize });
			}

		};
	}
}
