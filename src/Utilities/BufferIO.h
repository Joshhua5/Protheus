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
#include "smart_ptr.h"

namespace Pro {
	namespace Util {
		class BufferIO
		{
		protected:
			/*! Position in the buffer currently being access */
			unsigned head_;

			/*! Pointer to the buffer being accessed, used in BufferWriter/Reader*/
			smart_ptr<Buffer> buffer_;
			/*! Pointer to the buffer being accessed, used in AlignedWriter/Reader*/
			smart_ptr<AlignedBuffer> aligned_buffer_;

			/*! False if a simple pointer is passed */
			bool using_smart;
		public:
			BufferIO() {}
			~BufferIO() {}

			//! Sets the position of the @m_head
			void head(const unsigned position) {
				head_ = position;
			}


			//! Sets the position of the @m_head to 0
			void Reset() {
				head(0);
			}

			//! Returns the position of the @m_head
			unsigned head() const {
				return head_;
			}

			/*! Skips over the current @m_head position in bytes
				Used to progress the @m_head
			*/
			inline void Skip(const int position) {
				head_ += position;
			}

			//! Get's the internal buffers size
			unsigned size() const {
				return buffer_->size();
			}

			//! Finds the offset of the next deliminator
			unsigned Find(const char deliminator) const {
				// Make sure we only check inside the buffer
				char* buffer = (char*)buffer_->At(head_);
				char* buffer_end = (char*)buffer_->At(buffer_->size());

				int offset = 0;
				for (; *buffer != deliminator && buffer != buffer_end; ++buffer, ++offset);

				if (*buffer != deliminator)
					offset = 0;

				// include the deliminator 
				return ++offset;
			}
		};
	}
}