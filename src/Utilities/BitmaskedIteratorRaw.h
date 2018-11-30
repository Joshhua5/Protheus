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

#include "Bitmask.h"
#include "BufferIO.h"
#include "ClassDefinition.h"

namespace Pro {
	namespace Util {
		/*! Class to add on reading functionality to a Buffer and AlignedBuffer(Planned) */ 
		class BitmaskedIteratorRaw
		{
			size_t size_of_; 
			const Bitmask& bitmask_;
			char* pointer_;
			unsigned head_;
			unsigned end_;

		public: 
			BitmaskedIteratorRaw() = delete;

			BitmaskedIteratorRaw(void* start, const Bitmask& bitmask, size_t size, size_t sizeOf) :
				bitmask_(bitmask)
			{
				pointer_ = reinterpret_cast<char*>(start); 
				head_ = 0;
				size_of_ = sizeOf;
				end_ = size * sizeOf; 
			} 

			void* Read() {  
				while (!bitmask_.Check(head_++) && HasNext());
				if (!HasNext())
					return nullptr; 

				return reinterpret_cast<void*>(pointer_ + ((head_ - 1) * size_of_));
			}

			/*! False if @m_head is at the end of the buffer. */
			inline bool HasNext() const {
				return head_ < end_;
			}
		};
	}
}