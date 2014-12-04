/*************************************************************************
Protheus Source File.
Copyright (C), Protheus Studios, 2013-2014.
-------------------------------------------------------------------------

Description:
A extern class to provide io base functions for buffer writer/reader

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

			// sets the position of the IO
			void setPosition(const unsigned);


			// sets the position of the IO to 0
			void reset();

			// returns the position of the IO
			unsigned getPosition() const;

			// skips over the current IO position
			void skip(const int);

			// get's the internal buffers size
			unsigned getBufferSize() const;

			// Finds the offset of the next deliminator
			unsigned find(const char deliminator);
			 
		};
	}
}
