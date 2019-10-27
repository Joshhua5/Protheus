/*************************************************************************
Protheus Source File.
Copyright (C), Protheus Studios, 2013-2016.
-------------------------------------------------------------------------

-------------------------------------------------------------------------
History:
- 28:06:2014: Waring J.
*************************************************************************/
#pragma once
 
#include "Bitmask.h"  

namespace Pro {
	namespace Util {
		/*! Class to add on reading functionality to a Buffer and AlignedBuffer(Planned) */
		template<typename T>
		class BitmaskedIterator
		{
			T* pointer_;
			unsigned head_; 
			unsigned end_; 
			Bitmask& bitmask_;

		public:
			BitmaskedIterator() = delete;

			BitmaskedIterator(T* start, Bitmask& bitmask, size_t size) :
				bitmask_(bitmask)
			{
				pointer_ = reinterpret_cast<T*>(start);
				head_ = 0; 
				end_ = size;  
			}

			BitmaskedIterator(const BitmaskedIterator& rhs)
				: bitmask_(rhs.bitmask_) {
				pointer_ = rhs.pointer_;
				head_ = rhs.head_; 
				end_ = rhs.end_;
			}

			BitmaskedIterator(BitmaskedIterator&& rhs)
				: bitmask_(rhs.bitmask_) {
				pointer_ = rhs.pointer_;
				head_ = rhs.head_; 
				end_ = rhs.end_;
			}

			BitmaskedIterator& operator= (const BitmaskedIterator& rhs) {
				bitmask_ = rhs.bitmask_;
				pointer_ = rhs.pointer_;
				head_ = rhs.head_; 
				end_ = rhs.end_;
				return *this;
			}

			inline T* Read() { 
				//while (!bitmask_.Check(head_++) & head_ <= end_);
				if (++head_ <= end_)
					return pointer_ + (head_ - 1);
				return nullptr;

			}

			/*! False if @m_head is at the end of the buffer. */
			inline bool HasNext() const {
				return head_ < end_;
			}
		};
	}
}