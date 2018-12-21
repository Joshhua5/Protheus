#pragma once 
/*************************************************************************
Protheus Source File.
Copyright (C), Protheus Studios, 2013-2016.
-------------------------------------------------------------------------

-------------------------------------------------------------------------
History:
- 28:06:2014: Waring J.
*************************************************************************/ 

#include "BitmaskedIterator.h"
#include "LinkedArrayChunk.h"
#include "LinkedArray.h"

namespace Pro {
	namespace Util {
		using namespace std; 
		template<typename T>
		class LinkedArrayIterator
		{ 
			using ArrayChunk = typename Internal::ArrayChunk<T>;
			using array_chunk_iterator = typename vector<ArrayChunk>::iterator;

			array_chunk_iterator iterator_end;
			array_chunk_iterator iterator;
			BitmaskedIterator<T> chunk_iterator;

			inline bool LoadNextIterator() {
				iterator++;
				if (iterator == iterator_end)
					return false;
				chunk_iterator = std::move(iterator->GetIterator());
				return true;
			}

		public:
			LinkedArrayIterator() = delete;

			LinkedArrayIterator(LinkedArray<T>& target) : 
				iterator(target.internal_storage().begin()),
				iterator_end(target.internal_storage().end()),
				chunk_iterator(iterator->GetIterator())
			{ } 

			T* Read() {
				T* ptr = chunk_iterator.Read();
				if (ptr == nullptr) {
					if (LoadNextIterator())
						return Read();
					return nullptr;
				}
				return ptr;
			} 

			inline bool HasNext() {
				if (chunk_iterator.HasNext())
					return true;

				// If our object list is finished,
				// get the next iterator and check it
				if (LoadNextIterator())
					return HasNext();

				// We don't have an iterator and therefor no more objects
				return false;
			}
		};
	}
}