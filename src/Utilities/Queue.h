/*************************************************************************
Protheus Source File.
Copyright (C), Protheus Studios, 2013-2016.
-------------------------------------------------------------------------

Description:  
 
 Broken condititons:
 Push pop 1,000,000 objects onto a 1,000,000 queue
    999999 expected 0 Line: 40 Function: consumor
 Push pop 1,000,000 objects onto a 100,000 queue 
 
 Optimisation:
 Have the push and pop functions store a variable of how many pushs and pops
 are remaining before another check is required, this reduces the atomic access
 to size_ dramatically
 
 
-------------------------------------------------------------------------
History:
- 9:01:2015: Waring J.
*************************************************************************/
#pragma once
 
#include <memory>
#include <cstring>

namespace Pro {
	namespace Util {
		/*!  
			Queue resizes involves a lock.
		*/

        template<typename T>
		class alignas(64) Queue {
            size_t push_position_;
			size_t pop_position_;
            T* queue_;
            unsigned char queue_offset_;
			size_t capacity_;
			size_t size_;
            
			//! Checks if the @pos has overflown and then increments the @pos
            
            inline long CheckOverflow(size_t& pos) const{
                // if position is at the capacity then overflow
                if (pos == capacity_)
                    pos = 0;
                return pos;
            }
            
            //! Checks if the @pos has overflown and then increments the @pos
            inline long CheckOverflowWithIncrement(size_t& pos) const {
                // if position is at the capacity then overflow
                if (pos == capacity_) {
                    // 1 To account for the increment
                    pos = 1;
                    return 0;
                }
                return pos++;
            }

            inline bool DecrementSize(){
                if(size_ <= 0)
                    return false;
                --size_;
                return true;
            }
            
		public:
			Queue(const size_t size = 20480) {
				queue_ = reinterpret_cast<T*>(operator new(sizeof(T) * size + 64));
                queue_offset_ = 64 - ((size_t)queue_ % 64);
                queue_ = (T*)((char*)queue_ + queue_offset_);
				capacity_ = size;
				pop_position_ = push_position_ = size_ = 0;
		}

			Queue(const Queue& rhs) = delete;

			Queue(Queue&& rhs){
                queue_ = rhs.queue_;
                capacity_ = rhs.capacity_;
                pop_position_ = rhs.pop_position_;
                queue_offset_ = rhs.queue_offset_;
                push_position_ = rhs.push_position_;
                size_ = rhs.size_.load();
                
                rhs.queue_ = nullptr;
                rhs.size_ = 0;
                rhs.pop_position_ = 0;
                rhs.queue_offset_ = 0;
                rhs.push_position_ = 0;
                rhs.capacity_ = 0;
                rhs.being_resized_ = false;
            }
            
            Queue& operator=(Queue&& rhs){
                queue_ = rhs.queue_;
                capacity_ = rhs.capacity_;
                queue_offset_ = rhs.queue_offset_;
                pop_position_ = rhs.pop_position_;
                push_position_ = rhs.push_position_;
                size_ = rhs.size_;
                
                rhs.queue_ = nullptr;
                rhs.size_ = 0;
                rhs.pop_position_ = 0;
                rhs.queue_offset_ = 0;
                rhs.push_position_ = 0;
                rhs.capacity_ = 0;
                return *this;
            }
            
            Queue& operator=(const Queue& rhs) = delete; 

			~Queue() {
                while (Pop());
                queue_ = (T*)((char*)queue_ - queue_offset_);
				operator delete(queue_);
			}
  
            //! Resizes the queue to @size and moves all objects with memcpy
            //! Disabled by default due to being unsafe in a multithreaded application,
            //! Although in a single threaded application use the flag ENABLE_QUEUE_RESIZE to enable
            inline void Resize(const size_t size) { 
				// Create a new buffer aligned to 64B with enough space for the resize
                T* new_queue = reinterpret_cast<T*>(operator new(sizeof(T) * size + 64));
                unsigned new_queue_offset_ = 64 - ((size_t)new_queue % 64);
                new_queue = (T*)((char*)new_queue + new_queue_offset_);
                  
				if (push_position_ < pop_position_) {
					// Take the first half of the circular buffer and place it at the start
					std::memcpy(new_queue, queue_ + pop_position_, sizeof(T) * (capacity_ - pop_position_));
					// Append the second half of the circular buffer and place it adjacent
					std::memcpy(new_queue + (capacity_ - pop_position_), queue_, sizeof(T) * push_position_);
				}
				else
					std::memcpy(new_queue, queue_ + pop_position_, sizeof(T) * size_);
                
                operator delete((T*)((char*)queue_ - queue_offset_));
				capacity_ = size;
				pop_position_ = 0;
				push_position_ = size_;
                queue_ = new_queue;
                queue_offset_ = new_queue_offset_;
			}

			// Issue if pushing while poping with only one element
			inline bool Push(const T& obj) {
                if(capacity_ - size_  <= 1)
                    Resize(static_cast<size_t>(size_ * 1.2) + 5);
                auto pos = CheckOverflowWithIncrement(push_position_);

				new(reinterpret_cast<T*>(queue_) + pos) T(std::move(obj));
                ++size_;
                return true;
			}

			//! Pushes a new objects into the queue
            inline bool Push(T&& obj) {
                if(capacity_ - size_  <= 1)
                    Resize((size_ * 1.2) + 5);

				auto pos = CheckOverflowWithIncrement(push_position_);

				new(reinterpret_cast<T*>(queue_) + pos) T(std::move(obj));
                ++size_;
                return true;
			}
            

			//! Pushes a new objects into the queue, constructed with the provided arguments
			template<typename... Args>
            inline bool Emplace(Args... arguments) {
                if(capacity_ - size_ <= 1)
                    Resize((size_ * 1.2) + 5);
                
				auto pos = CheckOverflowWithIncrement(&push_position_);
				new(reinterpret_cast<T*>(queue_) + pos) T(arguments...);
                ++size_;
                return true;
            }

			//! Returns a copy to the next object being stored.
			//! Top doesn't wait for a resize to finish, due to its read only nature.
            inline const bool Top(const T& return_obj) const{
                if(size_ == 0)
                    return false;
                auto pos = CheckOverflow(pop_position_);
                return_obj = queue_[pos];
                return true;
			}

			//! Returns a copy to the next object being stored.
            inline bool Top(T& return_obj) {
                if(size_ == 0)
                    return false;
                auto pos = CheckOverflow(pop_position_);
				return_obj = queue_[pos];
                return true;
			}

			//! Removes the next object from the queue.
            inline bool Pop() {
                if(size_ == 0)
                    return false;
                
				// Get the next object
                auto pos = CheckOverflowWithIncrement(pop_position_);
				// Deconstruct object
				(queue_ + pos)->~T();
				// Top doesn't increment, so do that here 
                return DecrementSize();
			}
            
			//! Returns and removes the next object from the queue.
            //! if the queue is empty, false is returned and return_obj isn't modified
            inline bool TopPop(T& return_obj) {
                if(size_ == 0)
                    return false;
                
                // Get the next object
                size_t pos = CheckOverflowWithIncrement(pop_position_);
                // Create a copy before updating size, to prevent push from overwriting out object
                return_obj = std::move(*(queue_ + pos)); 
                return DecrementSize();
			}
            
            inline bool Empty() const { return size_ == 0; }
            
			//! Returns the count of objects being stored
			inline size_t size() const { return size_; }

			//! Returns the capacity of the queue before a resize is required.
			inline size_t capacity() const { return capacity_; }
 
		};
	}
}
