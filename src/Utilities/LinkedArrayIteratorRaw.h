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

#include "Bitmask.h" 
#include "LinkedArrayRaw.h"

namespace Pro {
	namespace Util {
		using namespace std;
		/*! The unique structure of a LinkedArrayRaw requires a new type of iterator capacble of
			seamlessly iterating over the chunks
		*/
		class LinkedArrayIteratorRaw
		{ 
			LinkedArrayRaw& linkedArray; 
			vector<LinkedArrayRaw::ArrayChunk>::iterator chunk_it;
			BitmaskedIteratorRaw object_it;
			 

			inline bool LoadNextIterator() { 
				if (chunk_it == linkedArray.chunks_.end())
					return false;
				chunk_it++;
				object_it = chunk_it->GetIterator(); 
				return true;
			}

		public:
			LinkedArrayIteratorRaw() = delete;

			LinkedArrayIteratorRaw(LinkedArrayRaw& target) 
				: linkedArray(target),
				  chunk_it(linkedArray.chunks_.begin()),
				  object_it(chunk_it->GetIterator())
			{ }

			void* Read() { 
				void* ptr = object_it.Read();
				if (ptr == nullptr) { 
					if (LoadNextIterator())
						return Read(); 
					return nullptr;
				}
				return ptr;
			}
			
			template<typename T>
			T* Read() {
				void* ptr = object_it.Read();
				if (ptr == nullptr) {
					if (LoadNextIterator())
						return Read();
					return nullptr;
				}
				return ptr;
			}
			 
			inline bool HasNext() {
				if (object_it.HasNext())
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