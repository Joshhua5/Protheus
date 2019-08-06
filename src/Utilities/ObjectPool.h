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
#include "LinkedList.h"
#include "Queue.h"
#include <math.h>

static const bool DEBUG_OBJECT_POOL_RT = false;

namespace Pro {
	namespace Util {
		/*! ObjectPool keeps track of memory allocated for objects
			and constructs a new object without allocation when requested. */

		//! An object pool stores all objects close to each other as a Cache optimisation
		//! This allows an ObjectPool to be used as an optimisation tool
        //! If the pool is deconstructed before all objects are released then
        //! Their memory will be released without deconstruction

		//! Memory is stored within blocks of objects, stored as a linked list 

		//! It wouldn't be a bad idea to have a object to wrap around the returned object to control
		//! the deallocation of the object.
		template<class T>
		class alignas(64) ObjectPool {
			// Each block contains block_size_ objects
			LinkedList<T*> objects;

			//! List of indexes which contain inactive objects, not leased
			Queue<unsigned> free_object_index_;
 
			// count of objects that this pool has served
            unsigned leased_objects_;
			unsigned short block_size_; 
            inline void AddBlock(){ 
                objects.PushBack(reinterpret_cast<T*>(operator new(sizeof(T) * block_size_)));
            }
            
			T* GetObject(){
                unsigned index;
                if(free_object_index_.TopPop(index) == false)
                    index = leased_objects_++;
                
                unsigned chunk_index = static_cast<unsigned>(floorl(index / block_size_));
                if(chunk_index > objects.size() - 1)
                    AddBlock();
                auto chunk = objects.At(chunk_index);
                if(DEBUG_OBJECT_POOL_RT)
                    printf("Chunk Ptr %p Chunk Index %i, index %i : %i", chunk, chunk_index, index % block_size_, index);
                return &chunk[index % block_size_];
			}

			// T* GetBlock(unsigned block_id) {
			// 		return objects.At(block_id);
			// }

			//! Returns -1 if the object doesn't belong to this pool
			//! Otherwise the index of the object in the pool is returned
			int BelongsTo(T* object) {
				auto start = objects.GetStartIterator();
				for (unsigned x = 0; start != nullptr; ++x) {
					unsigned size = static_cast<unsigned>(((char*)object - (char*)start->_ptr) / sizeof(T));
					if (size <= block_size_ && size >= 0)
						return (block_size_ * x) + size;
					start = start->_next;
				}
				return -1;
			}
		public: 
			ObjectPool(unsigned short block_size = 64){
                if(block_size == 0)
                    block_size = 1;
				block_size_ = block_size;
				leased_objects_ = 0;
                AddBlock();
			}
            
            ObjectPool(const ObjectPool&) = delete;
            ObjectPool& operator==(const ObjectPool&) = delete;

			~ObjectPool() {
				auto objs = objects.GetStartIterator();
				while (objs != nullptr){ 
					operator delete(objs->_ptr);
					objs = objs->_next; 
				} 
			}

			//! Stores a object as unused for reuse
			//! If this object didn't originate from the pool the object isn't modified
			void Return(T* obj) {
                if(DEBUG_OBJECT_POOL_RT)
                    printf("Returned: %p \n", obj);
                
				// Determine which object ID this pointer refers to
				// Check the pointers offset to each chunk
                int index = BelongsTo(obj);
				if (index != -1) {
					obj->~T();
					free_object_index_.Push(index);
				}
				--leased_objects_;
			}

			/*! Returns a old object, allocates a new one if the pool is empty */
			template<typename... Args>
			T* Get(Args&&... args) {
                T* object = GetObject();
                // If you're getting a compiler error here
                // It may be because you're passing incorrect parameters to Get
                // and there's no correct constuctor to call
                new(object)T(args...);
                 
                if(DEBUG_OBJECT_POOL_RT)
                    printf("Get: %p \n", object);
                
				return object;
			}

			//! Returns a old object, allocates a new one with the default constructor
			T* Get() {
                T* object = GetObject();
                new(object) T();
                
                if(DEBUG_OBJECT_POOL_RT)
                    printf("Get: %p \n", object);
                
				return object;
			}
            
            unsigned blockSize() const{
                return block_size_;
            }
            
            unsigned BlockCount() const{
                return objects.size();
            }  

			unsigned LeasedObjects() const {
				return leased_objects_;
			}
             
		};
	}
}