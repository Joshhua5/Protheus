#pragma once
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
 
#include "BitmaskedIteratorRaw.h"
#include "LinkedArrayRaw.h"

namespace Pro {
	namespace Util {
		using namespace std;
		/*! The unique structure of a LinkedArrayRaw requires a new type of iterator capacble of
			seamlessly iterating over the chunks
		*/
		class LinkedArrayIteratorRaw
		{  
			vector<LinkedArrayRaw::ArrayChunk>::iterator iterator_end;
			vector<LinkedArrayRaw::ArrayChunk>::iterator iterator;
			BitmaskedIteratorRaw chunk_iterator;
			  
			inline bool LoadNextIterator() {  
				if (iterator++ == iterator_end)
					return false;
				chunk_iterator = std::move(iterator->GetIterator());
				return true;
			}

		public:
			LinkedArrayIteratorRaw() = delete;

			LinkedArrayIteratorRaw(LinkedArrayRaw* target) 
				: iterator(target->chunks_.begin()),
				  iterator_end(target->chunks_.end()),
				  chunk_iterator(iterator->GetIterator())
			{ }

			void* Read() { 
				void* ptr = chunk_iterator.Read();
				if (ptr == nullptr) { 
					if (LoadNextIterator())
						return Read(); 
					return nullptr;
				}
				return ptr;
			} 
			
			template<typename T>
			T* Read() {
				void* ptr = chunk_iterator.Read();
				if (ptr == nullptr) {
					if (LoadNextIterator())
						return reinterpret_cast<T*>(Read());
					return nullptr;
				}
				return reinterpret_cast<T*>(ptr);
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