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

namespace Pro{
	namespace Util{
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
				write((void*) data, size);
			}
		};
	}
}



