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
 
#include "LinkedArrayChunk.h"

namespace Pro{
	namespace Util{

		template<typename T>
		class alignas(16) LinkedArray {  
			using ArrayChunk = typename Internal::ArrayChunk<T>;

			vector<ArrayChunk> chunks_; 
			using chunk_iterator = typename vector<ArrayChunk>::iterator; 

			unsigned block_size_; 

			inline void AddChunk(const unsigned chunkSize) {
				chunks_.push_back(ArrayChunk(chunkSize));
				memset(chunks_.back().chunk_.get(), 0, sizeof(T) * chunkSize);
			}

		public:
			LinkedArray(int block_size = 4096)
			{
				block_size_ = block_size; 
			}
			 
			T* At(size_t index) {
				for (ArrayChunk& chunk : chunks_) {
					if (chunk.ContainsIndex(index))
						return reinterpret_cast<T*>(chunk.GetObject(index));
					index -= chunk.Count();
				}
				return nullptr;
			}

			const T* At(size_t index) const {
				for (const ArrayChunk& chunk : chunks_) {
					if (chunk.ContainsIndex(index))
						return reinterpret_cast<T*>(chunk.GetObject(index));
					index -= chunk.Count();
				}
				return nullptr;
			}

			template<typename... args>
			T* Emplace(args... arg) {
				for (ArrayChunk& chunk : chunks_) {
					if (chunk.HasSpace()) {
						T* object = reinterpret_cast<T*>(chunks_.back().CreateObject());
						*object = T(arg...);
						return object;
					}
				}
				// We had no space and need to create a new chunk
				AddChunk(block_size_);
				T* object = reinterpret_cast<T*>(chunks_.back().CreateObject());
				*object = T(arg...);
				return object;
			}
			
			T* Append() {
				for (ArrayChunk& chunk : chunks_)
					if (chunk.HasSpace())
						return reinterpret_cast<T*>(chunks_.back().CreateObject()); 
				// We had no space and need to create a new chunk
				AddChunk(block_size_);
				return reinterpret_cast<T*>(chunks_.back().CreateObject());
			}
			  
			T* DeleteAt(size_t index) {
				for (ArrayChunk& chunk : chunks_) {
					if (chunk.ContainsIndex(index))
						return chunk.DestroyObject(index);
					index -= chunk.Count();
				}
				return nullptr;
			}

			//! Returns the count of objects currently stored
			inline size_t Count() {
				size_t total = 0;
				for (const auto & chunk : chunks_)
					total += chunk.actual_count_;
				return total;
			}

			inline float Fragmentation() {
				float fragmentation = 0;
				for (const auto & chunk : chunks_)
					fragmentation += ((float)chunk.actual_count_ / chunk.count_) / (float)chunks_.size();
				return fragmentation;
			} 
			
			//! pack arrays to fill in empty spaces, reduce memory usage and improved cache performance
			void Collapse() {
				for (ArrayChunk& chunk : chunks_) {
					chunk.Pack();
				}

				// Here we should do some cleanup of potentially empty chunks,
				// or consider cross chunk packing
			}

			void CollapseStep() {
				throw "Implement a single collapse on the next fragmented chunk";
			}



			vector<ArrayChunk>& internal_storage() { return chunks_; }

		};
	}
}