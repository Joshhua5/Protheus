#pragma once
/*************************************************************************
 Protheus Source File.
 Copyright (C), Protheus Studios, 2013-2016.
 -------------------------------------------------------------------------

 Description:

 -------------------------------------------------------------------------
 History:
 - 6:5:2015 Waring J.

 *************************************************************************/
 
#pragma once 
#include <vector>
#include "Iterator.h"

namespace Pro {
	namespace Util {
		/*! LinkedArrayRaw
			Thread Safe : False
			Lock Free : False

			TODO: How do we handle the removal of entries?
			if we remove one from a chunk in the middle, 
			Do we fill in the slot with a different entry?
			Do we shuffle all objects down?
			If we place a new entry there, we no longer have order
			
			Right now it looks like we shuffle all objects down and let the memory remain fragmented
			We can't shuffle down, though since memory references will be broken above that hole.

			Are there any other options that don't involve a bitmask for each entry?

			This data structure needs defined behaviour when it comes to breaking references, (ONLY on a @Collapse())

			I wrote a BitmaskedIterator to solve this, probably isn't optimal though
		*/   
		class LinkedArrayRaw {  
			struct Chunk {
				Chunk(size_t _size, size_t size_of) {
					capacity = _size;
					chunk = new char[_size * size_of];
					bitmask = new uint32_t[_size / 32];
				}
				~Chunk() {
					delete[] chunk;
					chunk = nullptr;
				}
				 
				size_t capacity; 
				uint32_t* bitmask;
				void* chunk; 
			};
			  
			std::vector<Chunk> nodes_;
			unsigned object_count_;
			unsigned block_size_;
			unsigned size_of_;

			inline void AddChunk(const unsigned chunkSize = 4096) {
				nodes_.push_back(Chunk(chunkSize, size_of_)); 
			} 

		public:
			LinkedArrayRaw(size_t sizeOf) {
				size_of_ = sizeOf; 
			}
			 
			void* At(unsigned index) {
				unsigned chunk_index = static_cast<unsigned>(floorl(index / block_size_));
				if (chunk_index > chunks_.size() - 1)
					return nullptr;
				auto chunk = chunks_.At(chunk_index);
				return &chunk[index % block_size_];
			}

			const void* At(unsigned index) const {
				unsigned chunk_index = static_cast<unsigned>(floorl(index / block_size_));
				if (chunk_index > chunks_.size() - 1)
					return nullptr;
				auto chunk = chunks_.At(chunk_index);
				return &chunk[index % block_size_];
			}

			inline void PushBack() {


				if (object_count_ == chunks_.size() * block_size_)
					AddBlock();
			}

			inline bool Empty() const {
				return object_count_ == 0;
			}

			void Collapse() {

			}

			inline bool IsValidIndex(size_t index) {
				return (index >= 0 && index <= object_count_ - 1);
			}

			inline unsigned ChunkSize() {
				return block_size_;
			}
		};
	}
}