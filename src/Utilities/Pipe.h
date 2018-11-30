/*************************************************************************
 Protheus Source File.
 Copyright (C), Protheus Studios, 2013-2016.
 -------------------------------------------------------------------------
 
 Description:
 A Pipe is identical to a queue, but with the assumption of single producor,
 single consumor.
 
 
 Optimisation:
 Have the push and pop functions store a variable of how many pushs and pops
 are remaining before another check is required, this reduces the atomic access
 to size_ dramatically
 
 
 -------------------------------------------------------------------------
 History:
 - 9:01:2015: Waring J.
 *************************************************************************/

#pragma once

#include <atomic>
#include <thread>
#include <memory>

namespace Pro {
    namespace Util {
        /*! Thread Safe Lock Free Pipe 
         */
        
        template<typename T>
        class alignas(64) Pipe {
        protected:
            // using unsigned long long to ensure that a overflow doesn't corrupt the data structure
            // A overflow of push/pop posititon would cause a jump in values returned from "posititon % capactity"
            unsigned long long push_position_;
            char pad_p[64 - (sizeof(unsigned long long))]; // Producer C-Line
			unsigned long long pop_position_;
            char pad_c[64 - (sizeof(unsigned long long))]; // Consumer C-Line
            T* queue_;
            size_t queue_offset_;
			size_t capacity_;
            char pad_sr[64 - sizeof(T*) - (sizeof(size_t) * 2)]; // Shared Read C-Line
            std::atomic<size_t> size_; 
            char pad_sw[64 - sizeof(std::atomic<size_t>)]; // Shared Write C-Line 
            
            inline bool DecrementSize(){
                if(size_ <= 0)
                    return false;
                size_.fetch_sub(1);
                return true;
            }
            
        public:
			//! If 0 is passed this than the pips will not be initialized allowing an array of pipes
			//! to be made on the stack without initializing them before required
			//! For situations where avoiding heap creation is required for memory alignment
			//! This approach requires that the Pipe is initialised before use  
			Pipe(const size_t size) {
                queue_ = nullptr;
				if(size != 0)
					Initialize(size);
            }
            
			Pipe(const Pipe& rhs) = delete; 
			Pipe& operator=(const Pipe& rhs) = delete;
            
			Pipe(Pipe&& rhs){
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
            }
            
			Pipe& operator=(Pipe&& rhs){
                queue_ = rhs.queue_;
                capacity_ = rhs.capacity_;
                queue_offset_ = rhs.queue_offset_;
                pop_position_ = rhs.pop_position_;
                push_position_ = rhs.push_position_;
                size_ = rhs.size_.load();
                
                rhs.queue_ = nullptr;
                rhs.size_ = 0;
                rhs.pop_position_ = 0;
                rhs.queue_offset_ = 0;
                rhs.push_position_ = 0;
                rhs.capacity_ = 0;
                return *this;
            }
            
            ~Pipe() {
				Destroy();
            }  

			//! Initializes the Pipe if created without initialisation
			//! Nullop if the data pointer is already populated
			inline void Initialize(const size_t size) {
				if (queue_ == nullptr) {
					queue_ = reinterpret_cast<T*>(operator new(sizeof(T) * size + 64));
					queue_offset_ = 64 - ((size_t)queue_ % 64);
					queue_ = (T*)((char*)queue_ + queue_offset_);
					capacity_ = size;
					pop_position_ = push_position_ = size_ = 0;
				}
			}

			//! A destroyed Pipe may be reused after calling Initialize() again.
			//! There is no check inplace to determine if the Pipe is initialised for pop and pushes 
			inline void Destroy() {
				if (queue_ == nullptr)
					return;
				while (Pop());
				queue_ = (T*)((char*)queue_ - queue_offset_);
				operator delete(queue_);
				queue_ = nullptr;
			}
            
            // Issue if pushing while poping with only one element
            inline bool Push(const T& obj) {
                if(capacity_ - size_ == 0)
                    return false;

                auto pos = push_position_++ % capacity_; 
                new(reinterpret_cast<T*>(queue_) + pos) T(obj);
                size_.fetch_add(1);
                return true;
            }
            
            //! Pushes a new objects into the queue
            inline bool Push(T&& obj) {
                if(capacity_ - size_ == 0)
                    return false;
                
                auto pos = push_position_++ % capacity_; 
                new(reinterpret_cast<T*>(queue_) + pos) T(std::move(obj));
                size_.fetch_add(1);
                return true;
            }
            
            
            //! Pushes a new objects into the queue, constructed with the provided arguments
            template<typename... Args>
            inline bool Emplace(Args... arguments) {
                if(capacity_ - size_  == 0)
                    return false;
                
				auto pos = push_position_++ % capacity_; 
                new(reinterpret_cast<T*>(queue_) + pos) T(arguments...);
                size_.fetch_add(1);
                return true;
            }
            
            //! Returns a copy to the next object being stored.
            //! Top doesn't wait for a resize to finish, due to its read only nature.
            inline const bool Top(const T& return_obj) const{
                if(size_ == 0)
                    return false;
                
                auto pos = pop_position_ % capacity_;
                return_obj = queue_[pos];
                return true;
            }
            
            //! Returns a copy to the next object being stored.
            inline bool Top(T& return_obj) {
                if(size_ == 0)
                    return false;
                
                auto pos = pop_position_ % capacity_;
                return_obj = queue_[pos];
                return true;
            }
            
            //! Removes the next object from the queue.
            inline bool Pop() {
                if(size_ == 0)
                    return false;
                
                // Get the next object
                auto pos = pop_position_++ % capacity_;
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
                size_t pos = pop_position_++ % capacity_;
                // Create a copy before updating size, to prevent push from overwriting out object
                return_obj = *(queue_ + pos);
                return DecrementSize();
            }
            
            inline bool Empty() const { return size_ == 0; }
            
            //! Returns the count of objects being stored
            inline size_t size() const { return size_; }
            
            //! Returns the capacity of the queue.
            inline size_t capacity() const { return capacity_; }
 
        };
    }
}