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
		using namespace std; 
		class Bitmask
		{ 
			template<typename T>
			struct array_deleter{ void operator ()(T const * p){ delete[] p; } }; 

			const size_t bitSizeUINT = 32;
			shared_ptr<uint32_t[]> bitmask_;
			size_t size_;
			 
		public:  
			Bitmask(const size_t size, const bool default_value) :
				bitmask_(new uint32_t[(size / bitSizeUINT) + 1], array_deleter<uint32_t>())
			{  
				memset(&bitmask_[0], (default_value) ? ~(uint32_t)0UL : (uint32_t)0UL, (size / bitSizeUINT + 1) * sizeof(uint32_t));
				size_ = size;
			} 

			Bitmask(Bitmask&& rhs) {
				bitmask_ = std::move(rhs.bitmask_);
				size_ = rhs.size_; 
				rhs.size_ = 0;
			} 

			Bitmask(const Bitmask& rhs)  {
				this->bitmask_ = rhs.bitmask_;
				this->size_ = rhs.size_; 
			}

			Bitmask& operator=(const Bitmask& rhs) { 
				this->bitmask_ = rhs.bitmask_;
				this->size_ = rhs.size_;
				return *this;
			}

			Bitmask& operator=(Bitmask&& rhs) { 
				this->bitmask_ = std::move(rhs.bitmask_);
				this->size_ = rhs.size_;  
				return *this;
			}
			  
			inline bool Check(const size_t index) const{ 
				if (index >= size_)
					return false;
				return (bitmask_[index / bitSizeUINT] >> index % bitSizeUINT) & uint32_t(1);
			}

			inline void Set(const size_t index, const bool value) { 
				if(value)
					bitmask_[index / bitSizeUINT] |= (uint32_t(1) << index % bitSizeUINT);
				else
					bitmask_[index / bitSizeUINT] &= ~(uint32_t(1) << index % bitSizeUINT); 
			} 

			template<bool value>
			inline void Set(const size_t index) { 
				if (value)
					bitmask_[index / bitSizeUINT] |= (uint32_t(1) << index % bitSizeUINT);
				else
					bitmask_[index / bitSizeUINT] &= ~(uint32_t(1) << index % bitSizeUINT);
			}
		};
	}
}