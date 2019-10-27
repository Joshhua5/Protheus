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

#include <functional>

#include "BitmaskedIterator.h"
 
namespace Pro {
	namespace Util {
		namespace Internal {
			using namespace std;
			template<typename T>
			struct alignas(64) ArrayChunk {
				unsigned int capacity_; // The maximum count of objects
				unsigned int count_; // The count of objects that have taken up space

				ArrayChunk(size_t _size) : mask_(_size, false)
				{
					capacity_ = _size;
					count_ = 0;
					actual_count_ = 0; 
					chunk_ = unique_ptr<T, function<void(T*)>>(new T[_size], [](T* ptr) {delete[] ptr; });
				}

				ArrayChunk(const ArrayChunk& rhs) = delete;
				const ArrayChunk& operator= (const ArrayChunk& rhs) = delete;

				ArrayChunk(ArrayChunk&& rhs) = default;
				ArrayChunk& operator= (ArrayChunk&& rhs) = default;

				unique_ptr<T, function<void(T*)>> chunk_;
				size_t actual_count_; // The count of objects that are active 
				Bitmask mask_; // Mask used to know which objects are active

				// TODO Do we allow for objects to be created within the empty places?
				// If we do, then replace the mask set to be the NextOffset of false
				T* CreateObject() {
					actual_count_++;
					mask_.Set<true>(count_);
					return chunk_.get() + count_++;
				}

				T* DestroyObject(const size_t index) {
					actual_count_--;
					mask_.Set<false>(mask_.GetOffset(index));
					return GetObject(index);
				}

				inline T* GetObject(const size_t index) {
					return chunk_.get() + mask_.GetOffset<true>(index);
				}

				inline const T* GetObject(const size_t index) const {
					return chunk_.get() + mask_.GetOffset<true>(index);
				}

				inline BitmaskedIterator<T> GetIterator() {
					return BitmaskedIterator<T>(chunk_.get(), mask_, capacity_);
				}

				inline bool IsPacked() const { return actual_count_ == capacity_; }
				inline size_t Count() const { return actual_count_; }
				inline bool HasSpace() const { return count_ < capacity_; }
				inline bool ContainsIndex(const size_t index) const { return actual_count_ > index; }

				void Pack() {
					if (IsPacked())
						return;

					// We want to shift everything down so that all empty spaces are filled
					auto iterator = GetIterator();
					auto writingHead = chunk_.get();
					for (size_t i = 0; i < capacity_; ++i) {
						// Find the next empty spot
						if (!mask_.Check(i) && iterator.HasNext()) {
							// Get the next object in memory
							auto nextObject = iterator.Read();

							// If we're at the end of our iterator, then there's nothing left to do
							if (nextObject == nullptr)
								return;

							// Move the object
							memcpy(writingHead, nextObject, sizeof(T));

							// Update the masks
							mask_.Set<true>((size_t)(writingHead - chunk_.get()));
							mask_.Set<false>((size_t)(nextObject - chunk_.get()));

							// Update the object counts, since we now have space at the end of the chunk
							writingHead++;
							count_--;
						}
					}
				}
			};
		}
	}
}