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
#include "Buffer.h" 
#include "AlignedBuffer.h"
#include "smart_pointer.h"

namespace Pro {
	namespace Util {
		class BufferIO
		{
		protected:
			/*! Position in the buffer currently being access */
			unsigned m_head;
			
			union {
				/*! Pointer to the buffer being accessed, used in BufferWriter/Reader*/
				smart_pointer<Buffer> m_buffer;
				/*! Pointer to the buffer being accessed, used in AlignedWriter/Reader*/
				smart_pointer<AlignedBuffer> aligned_buffer;
			};
			/*! False if a simple pointer is passed */
			bool using_smart;
		public:
			BufferIO(){}
			~BufferIO(){}

			//! Sets the position of the @m_head
			void setPosition(const unsigned position) {
				m_head = position;
			}


			//! Sets the position of the @m_head to 0
			void reset() {
				setPosition(0);
			}

			//! Returns the position of the @m_head
			unsigned getPosition() const {
				return m_head;
			}

			/*! Skips over the current @m_head position in bytes
				Used to progress the @m_head
			*/
			inline void skip(const int position) {
				m_head += position;
			}

			//! Get's the internal buffers size
			unsigned getBufferSize() const {
				return m_buffer._ptr->size();
			}

			//! Finds the offset of the next deliminator
			unsigned find(const char deliminator) const {
				// Make sure we only check inside the buffer
				char* buf = (char*)m_buffer->at(m_head);
				char* bufferEnd = (char*)m_buffer->at(m_buffer->size());

				int offset = 0;
				for (; *buf != deliminator && buf != bufferEnd; ++buf, ++offset);

				if (*buf != deliminator)
					offset = 0;

				// include the deliminator 
				return ++offset;
			}
		};
	}
}