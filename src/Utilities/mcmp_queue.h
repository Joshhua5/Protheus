/*************************************************************************
 Protheus Source File.
 Copyright (C), Protheus Studios, 2013-2016.
 -------------------------------------------------------------------------
 
 Description:
 A multiple consumer, multiple producer queue used to communicate between various threads
 with no limit on which thread can push and pop from the queue
 
 -------------------------------------------------------------------------
 History:
 - 9:01:2015: Waring J.
 *************************************************************************/

#pragma once

#include "Pipe.h"
#include <mutex>

namespace Pro {
    namespace Util {
        
        template<typename T>
        class alignas(64) mcmp_queue {
            
            struct alignas(64) sub_queue : public Pipe<T> {
                sub_queue(void* data, size_t size) : Pipe<T>(0){
                    Pipe<T>::queue_ = (T*)data;
                    Pipe<T>::capacity_ = size;
                    Pipe<T>::pop_position_ = Pipe<T>::push_position_ = Pipe<T>::size_ = 0;
                }
                
                ~sub_queue() {
                    while (Pipe<T>::Pop());
                    Pipe<T>::queue_ = nullptr;
                    Pipe<T>::Destroy();
                }
            };
            
            struct Flag {
                Flag() { active = false; }
                std::atomic<bool> active;
                // Padding so that there's a cache miss only
                // when a hwthread enteres the queue of another hwthread
                char padding[64 - sizeof(std::atomic<bool>)];
            };
            
            //! Flags to signify if another thread is already in this queue
            Flag* flags_pop_;
            Flag* flags_push_;
            sub_queue* queues_;
            unsigned queue_offset_;
            unsigned capacity_;
            unsigned short sub_queue_count;
            
            std::atomic<unsigned long long> active_sub_queue_pop;
            std::atomic<unsigned long long> active_sub_queue_push;
            std::atomic<size_t> size_;
            
            
            //The queue is broken into subqueues
            // Each subqueue can only have one parallel push and pop from it at a time
            // Between each queue is a 64byte padding
        public:
            //! @size may be changed to be a multiple of @sub_queue_count, @size will only be increased
            //! If sub_queue_count is 0 than the comptuers maximum concurrent thread count * 4 will be used
            mcmp_queue(unsigned size, unsigned sub_queue_count) {
                if (sub_queue_count == 0)
                    sub_queue_count = std::thread::hardware_concurrency() * 4;
                // Set the size to a higher multiple of the queue count
                size -= size % sub_queue_count;
                size += sub_queue_count;
                
                // The first section of the heap contains queue primate members
                // The second contains the data that the queues point to in their data with padding between
                queues_ = (sub_queue*)operator new((sizeof(sub_queue) * sub_queue_count) + size * sizeof(T) + (64 * sub_queue_count) + 64);
                flags_pop_ = new Flag[sub_queue_count];
                flags_push_ = new Flag[sub_queue_count];
                // char because our pointer arithmetic works with the assuming of 1 == 1 byte
                char* start_data_chunk = (char*)(queues_ + sub_queue_count);
                unsigned queue_size = size / sub_queue_count;
                unsigned data_chunk_size = (queue_size * sizeof(T)) + 64;
                
                for (unsigned x = 0; x < sub_queue_count; ++x)
                    new(queues_ + x) sub_queue(start_data_chunk + (x * data_chunk_size), queue_size);
                
                active_sub_queue_pop = 0;
                active_sub_queue_push = 0;
                size_ = 0;
                this->capacity_ = size;
                this->sub_queue_count = sub_queue_count;
            }
            
            ~mcmp_queue() {
                for (unsigned x = 0; x < sub_queue_count; x++)
                    (queues_ + x)->~sub_queue();
                operator delete(queues_);
                delete[] flags_pop_;
                delete[] flags_push_;
            }
            
            //! Returns false if there was no object to return
            //! object is set to the returned object
            bool Pop(T& object) {
                if(size_ == 0)
                    return false;
                unsigned long long pos;
                bool locked;
                
                // Check if this queue is already active
                do{
                    bool fal = false;
                    pos = active_sub_queue_pop.fetch_add(1) % sub_queue_count;
                    locked = flags_pop_[pos].active.compare_exchange_weak(fal, true);
                    if(locked == true && queues_[pos].size() == 0){
                        // Skip over this queue
                        flags_pop_[pos].active = false;
                        locked = false;
                    }
                }while(locked == false);
                
                // Perform a pop on this queue
                bool result = queues_[pos].TopPop(object);
                if(result){
                    // We successfully operated on this queue so we'll increment
                    // the size and queue
                    size_.fetch_sub(1);
                }
                flags_pop_[pos].active.store(false);
                return result;
            }
            
            //! The object is copied into the queue
            //! returns true if successful
            bool Push(const T& object) {
                if(size_ == capacity_)
                    return false;
                unsigned long long pos;
                bool locked;
                
                // Check if this queue is already active
                do
                {
                    bool fal = false;
                    pos = active_sub_queue_push.fetch_add(1) % sub_queue_count;
                    locked = flags_push_[pos].active.compare_exchange_weak(fal, true);
                    if(locked == true && queues_[pos].size() == queues_[pos].capacity()){
                        // Skip over this queue
                        flags_push_[pos].active = false;
                        locked = false;
                    }
                } while(locked == false);
                
                // Perform a push on this queue
                bool result = queues_[pos].Push(object);
                
                // If our push was successful than we update the mcmp queue
                if(result == true){
                    size_.fetch_add(1);
                }
                flags_push_[pos].active.store(false);
                return result;
            }
            
            //! Return the number of internal queues,
            //! Each queue can support 1 async push
            //! Therefor 4 queues can support 4 pushes simultaneously
            inline unsigned short QueueCount() const{
                return sub_queue_count;
            }
            
            inline size_t size() const{
                return size_;
            }
            
            inline size_t capacity() const{
                return capacity_;
            }
            
            void DebugInfo() const {
                for (unsigned x = 0; x < sub_queue_count; ++x)
                    printf("queue:%i size:%i capacity:%i \n", x, (int)queues_[x].size(), (int)queues_[x].capacity());
            }
        };
    }
}
