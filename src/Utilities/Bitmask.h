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
#include <bitset>

#include "BufferIO.h"
#include "ClassDefinition.h"

namespace Pro {
	namespace Util {
		using namespace std; 
		class Bitmask
		{   
			const static size_t BitSetSize = 256; // half a cacheline
			shared_ptr<bitset<BitSetSize>[]> bitmask_;
			size_t size_;
			 
		public:  
			Bitmask(const size_t size, const bool default_value) :
				bitmask_(new bitset<BitSetSize>[(size / BitSetSize) + 1], [](bitset<BitSetSize>* ptr) {delete[] ptr; })
			{  
				size_ = size;
				if(default_value)
					for (unsigned i = 0; i < size / BitSetSize; ++i)
						bitmask_[i].set(); 
				else 
					for (unsigned i = 0; i < size / BitSetSize; ++i)
						bitmask_[i].reset(); 
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

			const Bitmask& operator=(const Bitmask& rhs) { 
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
				return bitmask_[index / BitSetSize].test(index % BitSetSize); 
			}
			 
			// Returns the index to the nth instance of the value
			template<bool value = true>
			inline size_t GetOffset(long index) const {
				size_t offset = 0;
				while (index != -1) {
					if (bitmask_[offset / BitSetSize].test(offset % BitSetSize))
						--index;
					++offset;
				}
				return offset - 1;
			}

			// TODO does the template help optimization? should check the assembily
			template<bool value>
			inline void Set(const size_t index) {  
				bitmask_[index / BitSetSize].set(index % BitSetSize, value);
			}
			
			inline void Set(const size_t index, const bool value) {
				bitmask_[index / BitSetSize].set(index % BitSetSize, value);
			} 
		};

		 
	}
}