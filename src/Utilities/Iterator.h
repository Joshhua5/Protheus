/*************************************************************************
Protheus Source File.
Copyright (C), Protheus Studios, 2013-2016.
-------------------------------------------------------------------------

-------------------------------------------------------------------------
History:
- 28:06:2014: Waring J.
*************************************************************************/
#pragma once

#include <iterator>

#include "BufferIO.h"
#include "ClassDefinition.h"

namespace Pro {
	namespace Util {
		/*! Class to add on reading functionality to a Buffer and AlignedBuffer(Planned) */
		template<typename T>
		class Iterator
		{
			T* head;
			T* end;

		public: 
			Iterator(const Iterator& rhs) { head = rhs.head; end = rhs.end; } 
			Iterator(T* data, const size_t size) {
				head = data;
				end = head + size;
			}

			~Iterator() {
				head = end = nullptr;
			}
			 
			T* Read() {
				if (head == end)
					return nullptr;
				return head++; 
			}
			 
			/*! False if @m_head is at the end of the buffer. */
			bool HasNext() const {
				return head != end;
			}
		};
	}
}