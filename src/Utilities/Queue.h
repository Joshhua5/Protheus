/*************************************************************************
Protheus Source File.
Copyright (C), Protheus Studios, 2013-2014.
-------------------------------------------------------------------------

Description:
A extern class to provide writing functions to a buffer
Not safe for 2 simultaneous producers or 2 simultaneous consumors 

Note: 
	Does not produce log data due to a dependency.
	Log writes have been replaces with throws enables with the NDEBUG flag
-------------------------------------------------------------------------
History:
- 9:01:2015: Waring J.
*************************************************************************/
#pragma once

#include <atomic>
#include <mutex> 
 
namespace Pro {
	namespace Util {
		/*! Thread Safe Lock Free Queue
			Queue resizes involves a lock.

			TODO:	Unsafe to push and pop during a resize, must prevent them.
		*/

		template<typename T>
		class Queue {
			std::atomic<T*> queue_;
			std::atomic<size_t> pop_position_;
			std::atomic<size_t> push_position_;
			std::atomic<size_t> size_;
			std::atomic<size_t> capacity_;
			std::mutex resize_lock_;

			inline size_t CheckOverflow(std::atomic<size_t>* pos) {
				// if position is at the capacity then overflow
				if (pos->load() == capacity_) {
					pos->store(0);
					return 0;
				}
				return *pos;
			}

			inline size_t CheckOverflowWithIncrement(std::atomic<size_t>* pos) {
				// if position is at the capacity then overflow
				if (pos->load() == capacity_) {
					// 1 To account for the increment
					pos->store(1);
					return 0;
				}
				return (*pos)++;
			}

		public:
			Queue(const size_t size = 64) {
				queue_ = reinterpret_cast<T*>(operator new(sizeof(T) * size));
				capacity_ = size;
				pop_position_ = push_position_ = size_ = 0;
			}
			~Queue() {
				resize_lock_.lock();
				while (!Empty())
					Pop();
				operator delete(queue_.load());
				resize_lock_.unlock();
			}

			//! Resize is not thread safe
			inline void Resize(const size_t size) {
				// TODO If a resize has been performed making it larger than the requested resize then exit after the mutex
				std::lock_guard<std::mutex> lk(resize_lock_);

				if (size <= size_)
					// Either another thread has already resized or the requested size is less than the stored size
					return;

				auto old_queue = queue_.load();
				auto new_queue = reinterpret_cast<T*>(operator new (sizeof(T) * size));

				const size_t sizem = capacity_ - 1;

				// Problem with the pop and push positions
				if (push_position_ < pop_position_) {
					memcpy(new_queue, old_queue + pop_position_, sizeof(T) * (sizem - pop_position_));
					memcpy(new_queue + (sizem - pop_position_), old_queue, sizeof(T) * push_position_);
				}
				else
					memcpy(new_queue + pop_position_, old_queue, sizeof(T) * (push_position_ - pop_position_));

				capacity_ = size;
				queue_.store(new_queue);
				pop_position_ = 0;
				push_position_ = size_.load();
				operator delete(old_queue);
			}

			// Issue if pushing while poping with only one element
			inline void Push(const T& obj) {
				if (size_ == capacity_ - 1)
					Resize(static_cast<size_t>(capacity_ * 1.2f));
				auto pos = CheckOverflowWithIncrement(&push_position_);

				new(reinterpret_cast<T*>(queue_.load()) + pos) T(std::move(obj)); 
				++size_;
			}

			inline void Push(T&& obj) {
				if (size_ == capacity_ - 1)
					Resize(static_cast<size_t>(capacity_ * 1.2f));
				auto pos = CheckOverflowWithIncrement(&push_position_);

				new(reinterpret_cast<T*>(queue_.load()) + pos) T(std::move(obj)); 
				++size_; 
			}

			template<typename... Args>
			inline void Emplace(Args... arguments) {
				if (size_ == capacity_ - 1)
					Resize(static_cast<size_t>(capacity_ * 1.2f));
				auto pos = CheckOverflowWithIncrement(&push_position_); 
				new(reinterpret_cast<T*>(queue_.load()) + pos) T(arguments);
				++size_;
			}
			 

			inline T& Top() {  
				if (Empty()) {
#if NDEBUG
					throw "Called Top on a empty queue, undefined returned object.";
#endif 
				}
				auto pos = CheckOverflow(&pop_position_); 
				return queue_.load()[pos]; 
			}
			 
			inline void Pop() { 
				// Get the next object
				auto pos = pop_position_.load();
				if (pos == capacity_) {
					pop_position_.store(1);
					pos = 0;
				}
				else
					pop_position_++;
				// Deconstruct object
				(queue_.load() + pos)->~T(); 
				// Top doesn't increment, so do that here 
				--size_;
			}

			inline T TopPop() {
				T returnObj(Top());
				Pop();
				return returnObj;
			}

			inline bool Empty() const { return size_.load() == 0; }

			inline size_t size() const { return size_; }

			inline size_t capacity() const { return capacity_; }
		};
	}
}