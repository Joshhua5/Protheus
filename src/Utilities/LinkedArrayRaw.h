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
#include "Bitmask.h"
#include "BitmaskedIteratorRaw.h"
#include "LinkedArrayIteratorRaw.h"

namespace Pro {
	namespace Util {
		/*! LinkedArrayRaw
			Thread Safe : False
			Lock Free : False

			Raw Structures don't call any deconstructors

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
		
		class alignas(16) LinkedArrayRaw {
			friend class LinkedArrayIteratorRaw;
			struct alignas(16) ArrayChunk {
				ArrayChunk(size_t _size, size_t size_of)
					: mask_(_size, false)
				{
					capacity_ = _size;
					count_ = 0;
					size_of_ = size_of;
					chunk_ = new char[_size * size_of]; 
				}
				~ArrayChunk() {
					delete[] chunk_;
					chunk_ = nullptr;
				}
				 
				char* chunk_; // Memory of the objects
				size_t capacity_; // The maximum count of objects
				size_t count_; // The count of objects that have taken up space
				size_t actual_count_; // The count of objects that are active
				size_t size_of_; // Size of an object in bytes
				Bitmask mask_; // Mask used to know which objects are active
				 
				void* CreateObject(){  
					actual_count_++;
					mask_.Set<true>(count_);
					return chunk_ + (size_of_ * (count_++));
				} 

				void* DestroyObject(const size_t index) {
					actual_count_--;
					mask_.Set<false>(index);
					return chunk_ + (size_of_ * (index));
				}

				inline void* GetObject(const size_t index) {
					// Incorrect since we need to consider bitmasks
					return chunk_ + (size_of_ * (index));
				}

				inline const void* GetObject(const size_t index) const {
					// Incorrect since we need to consider bitmasks 
					return chunk_ + (size_of_ * (index));
				}

				inline BitmaskedIteratorRaw GetIterator() {
					return BitmaskedIteratorRaw(chunk_, mask_, capacity_, size_of_);
				}

				inline bool IsPacked() const { return actual_count_ == capacity_; }
				inline size_t Count() const { return actual_count_; }
				inline bool HasSpace() const { return count_ < capacity_; }
				inline bool ContainsIndex(const size_t index) const { return actual_count_ > index; } 

				void Pack(){
					if (IsPacked())
						return;

					// We want to shift everything down so that all empty spaces are filled
					auto iterator = GetIterator();
					char* writingHead = chunk_;
					for (size_t i = 0; i < capacity_; ++i) {
						// Find the next empty spot
						if (!mask_.Check(i) && iterator.HasNext()) {
							// Get the next object in memory
							char* nextObject = reinterpret_cast<char*>(iterator.Read());

							// If we're at the end of our iterator, then there's nothing left to do
							if (nextObject == nullptr)
								return;

							// Move the object
							memcpy(writingHead, nextObject, size_of_); 
							writingHead += size_of_;

							// Update the masks
							mask_.Set<true>((size_t)(writingHead - chunk_));
							mask_.Set<false>((size_t)(nextObject - chunk_));

							// Update the object counts, since we now have space at the end of the chunk
							count_--;
						}
					} 
				}
			};
			  
			std::vector<ArrayChunk> chunks_;
			unsigned object_count_;
			unsigned block_size_;
			unsigned size_of_;

			inline void AddChunk(const unsigned chunkSize) {
				chunks_.push_back(ArrayChunk(chunkSize, size_of_));
			} 
			
			friend class LinkedArrayIteratorRaw;
		public:
			LinkedArrayRaw(size_t sizeOf, int block_size = 4096) {
				block_size_ = block_size;
				size_of_ = sizeOf; 
			}
			 
			void* At(size_t index) { 
				for (ArrayChunk& chunk : chunks_) {
					if (chunk.ContainsIndex(index))
						return chunk.GetObject(index);
					index -= chunk.Count();
				}
				return nullptr;
			}

			const void* At(size_t index) const {
				for (const ArrayChunk& chunk : chunks_) {
					if (chunk.ContainsIndex(index))
						return chunk.GetObject(index);
					index -= chunk.Count();
				}
				return nullptr;
			}

			template<typename T>
			T* At(size_t index) {
				for (const ArrayChunk& chunk : chunks_) {
					if (chunk.ContainsIndex(index))
						return reinterpret_cast<T*>(chunk.GetObject(index));
					index -= chunk.Count();
				}
				return nullptr;
			}
			
			template<typename T>
			const T* At(size_t index) const {
				for (const ArrayChunk& chunk : chunks_) {
					if (chunk.ContainsIndex(index))
						return reinterpret_cast<T*>(chunk.GetObject(index));
					index -= chunk.Count();
				}
				return nullptr;
			}

			template<typename T, typename... args>
			T* Insert(args...) {
				for (ArrayChunk& chunk : chunks_) {
					if (chunk.HasSpace()) {
						*chunk.CreateObject() = T(args...);
						return;
					}
				}
				// We had no space and need to create a new chunk
				AddChunk(block_size_);
				T* object = reinterpret_cast<T*>(chunks_.back().CreateObject());
				*object = T(args...);
				return object;
			}
			
			void* Insert() {
				for (ArrayChunk& chunk : chunks_) {
					if (chunk.HasSpace()) {
						return chunk.CreateObject(); 
					}
				}
				AddChunk(block_size_);
				return chunks_.back().CreateObject();
			}

			LinkedArrayIteratorRaw GetIterator() { 
				return LinkedArrayIteratorRaw(*this);
			}
			 
			//! pack arrays to fill in empty spaces, reduce memory usage and improved cache performance
			void Collapse() {
				for (ArrayChunk& chunk : chunks_) {
					chunk.Pack();
				} 

				// Here we should do some cleanup of potentially empty chunks,
				// or consider cross chunk packing
			}

			inline bool IsValidIndex(size_t index) {
				return (index >= 0 && index <= object_count_ - 1);
			}

			void* Delete(size_t index) { 
				for (ArrayChunk& chunk : chunks_) {
					if (chunk.ContainsIndex(index))
						return chunk.DestroyObject(index);
					index -= chunk.Count(); 
				}
				return nullptr;
			}

			inline size_t Count() {
				size_t total = 0;
				for (const auto & chunk : chunks_)
					total += chunk.actual_count_; 
				return total;
			}

			inline float Fragmentation() {
				float fragmentation = 0;
				for (const auto & chunk : chunks_)
					fragmentation += (chunk.count_ / (float)chunk.actual_count_) / (float)chunks_.size();
				return fragmentation;
			} 
		};
	}
}