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


namespace Pro{
	namespace Util{
		template<typedef T, unsigned chunk_size = 64>
		class LinkedArray{  

			ArrayList<ObjectChunk> references_;
            LinkedList<T*> blocks_;
			unsigned object_count_;
            unsigned block_size_;
  
            inline void AddBlock(){
                objects.PushBack(reinterpret_cast<T*>(operator new(sizeof(T) * block_size_)));
            }

		public:

            T* At(unsigned index){
                unsigned chunk_index = static_cast<unsigned>(floorl(index / block_size_));
                if(chunk_index > chunks_.size() - 1)
                    return nullptr;
                auto chunk = chunks_.At(chunk_index);
                return &chunk[index % block_size_];
            }
            
            const T* At(unsigned index) const{
                unsigned chunk_index = static_cast<unsigned>(floorl(index / block_size_));
                if(chunk_index > chunks_.size() - 1)
                    return nullptr;
                auto chunk = chunks_.At(chunk_index);
                return &chunk[index % block_size_];
            }
            
            
			inline void PushBack(T& object){
				if(object_count_ == chunks_.size() * block_size_)
                    AddBlock();
			}

			inline bool Empty() const{
				return object_count_ == 0;
			}

			inline bool IsValidIndex(size_t index){
				return (index >= 0 && index <= object_count_ - 1);
			}
                            
                            inline T* Block(unsigned index){
                                if(index > chunks_.size())
                                    return nullptr;
                                return chunks_.At(index);
                            }
                            
                            inline unsigned ChunkSize(){
                                return block_size_;
                            }
                            
                        
		};
	}
}