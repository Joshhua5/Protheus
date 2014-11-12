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
			 
		};
	}
}
