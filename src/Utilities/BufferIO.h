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


		// sets the position of the writer
		void BufferIO::setPosition(const unsigned position) {
			m_head = position;
		}

		// sets the position of the IO to 0
		void BufferIO::reset() {
			setPosition(0);
		}

		// returns the position of the writer
		unsigned BufferIO::getPosition() const {
			return m_head;
		}
		// skips over the current writing position
		void BufferIO::skip(const int position) {
			m_head += position;
		}
		// get's the internal buffers size
		unsigned BufferIO::getBufferSize() const {
			return m_buffer->size();
		}
		unsigned BufferIO::find(const char deliminator) {
			// Make sure we only check inside the buffer
			register auto buf = (char*)m_buffer->at(m_head);
			register auto bufferEnd = (char*)m_buffer->at(m_buffer->size());
			int offset = 0;
			for (; *buf != deliminator && buf != bufferEnd; ++buf, ++offset);

			if (*buf != deliminator)
				offset = 0;

			// include the deliminator 
			return ++offset;
		}

}
