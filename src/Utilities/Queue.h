/*************************************************************************
Protheus Source File.
Copyright (C), Protheus Studios, 2013-2015.
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
			*/

		template<typename T>
		class Queue {
			std::atomic<T*> queue_;
			std::atomic<size_t> pop_position_;
			std::atomic<size_t> push_position_;
			std::atomic<size_t> size_;
			std::atomic<size_t> capacity_;
			std::atomic<bool> being_resized_;
			std::mutex resize_lock_;

			// Checks if the @pos has overflown and resets it's position if true
			inline size_t CheckOverflow(std::atomic<size_t>* pos) const{
				// if position is at the capacity then overflow
				if (pos->load() == capacity_) {
					pos->store(0);
					return 0;
				}
				return *pos;
			}
			
			//! Checks if the @pos has overflown and then increments the @pos
			inline size_t CheckOverflowWithIncrement(std::atomic<size_t>* pos) const {
				// if position is at the capacity then overflow
				if (pos->load() == capacity_) {
					// 1 To account for the increment
					pos->store(1);
					return 0;
				}
				return (*pos)++;
			}

			//! If a resize is being performed then wait until the resize is finished
			inline void WaitForResize() const{ 
#ifdef PRO_PROFILE
				if (being_resized_){
					global_log.Report<LogCode::PERFORMANCE>("Push and Pops waiting on resize, suggest reserving more data.", __FUNCTION__, __LINE__);
					while (being_resized_)
						std::this_thread::yield(); 
				}
#else
				while (being_resized_)
					std::this_thread::yield(); 
#endif 
				}

		public:
			Queue(const size_t size = 64) {
				queue_ = reinterpret_cast<T*>(operator new(sizeof(T) * size));
				capacity_ = size;
				being_resized_ = false;
				pop_position_ = push_position_ = size_ = 0;
			}
			~Queue() {
				// Lock to make sure nothing is resizing
				std::lock_guard<std::mutex> lk(resize_lock_); 
				while (!Empty())
					Pop();
				operator delete(queue_.load()); 
			}

			//! Resize will block all Push's and Pops unless their constant.
			inline void Resize(const size_t size) {
				std::lock_guard<std::mutex> lk(resize_lock_);

				if (size <= size_)
					// Either another thread has already resized or the requested size is less than the stored size
					return;

				being_resized_ = true;

				auto old_queue = queue_.load();
				auto new_queue = reinterpret_cast<T*>(operator new (sizeof(T) * size));

				const size_t sizem = capacity_ - 1;

				// Problem with the pop and push positions
				if (push_position_ < pop_position_) {
					memcpy(new_queue, old_queue + pop_position_, sizeof(T) * (sizem - pop_position_ + 1));
					memcpy(new_queue + (sizem - pop_position_ + 1), old_queue, sizeof(T) * push_position_);
				}
				else
					memcpy(new_queue + pop_position_, old_queue, sizeof(T) * (push_position_ - pop_position_));

				capacity_ = size;
				queue_.store(new_queue);
				pop_position_ = 0;
				push_position_ = size_.load();
				operator delete(old_queue);

				being_resized_ = false;
			}

			// Issue if pushing while poping with only one element
			inline void Push(const T& obj) {
				if (size_ == capacity_ - 1)
					Resize(static_cast<size_t>(capacity_ * 1.2f));
				WaitForResize();
				auto pos = CheckOverflowWithIncrement(&push_position_);

				new(reinterpret_cast<T*>(queue_.load()) + pos) T(std::move(obj));
				++size_;
			}

			//! Pushes a new objects into the queue
			inline void Push(T&& obj) {
				if (size_ == capacity_ - 1)
					Resize(static_cast<size_t>(capacity_ * 1.2f)); 
				WaitForResize();

				auto pos = CheckOverflowWithIncrement(&push_position_);

				new(reinterpret_cast<T*>(queue_.load()) + pos) T(std::move(obj));
				++size_;
			}

			//! Pushes a new objects into the queue, constructed with the provided arguments
			template<typename... Args>
			inline void Emplace(Args... arguments) {
				if (size_ == capacity_ - 1)
					Resize(static_cast<size_t>(capacity_ * 1.2f));
				WaitForResize();
				auto pos = CheckOverflowWithIncrement(&push_position_);
				new(reinterpret_cast<T*>(queue_.load()) + pos) T(arguments);
				++size_;
			}

			//! Returns reference to the next object being stored.
			//! Top doesn't wait for a resize to finish, due to its read only nature.
			inline const T& Top() const{
				if (Empty()) {
#if PRO_DEBUG
					throw "Called Top on a empty queue, undefined returned object.";
#endif 
				} 
				auto pos = CheckOverflow(&pop_position_);
				return queue_.load()[pos];
			}

			//! Returns reference to the next object being stored.
			inline T& Top() {
				if (Empty()) {
#if PRO_DEBUG
					throw "Called Top on a empty queue, undefined returned object.";
#endif 
				}
				WaitForResize();
				auto pos = CheckOverflow(&pop_position_);
				return queue_.load()[pos];
			}

			//! Removes the next object from the queue.
			inline void Pop() {
				WaitForResize();

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

			//! Returns and removes the next object from the queue.
			inline T TopPop() {
				T returnObj(Top());
				Pop();
				return returnObj;
			}

			//! Returns true if no objects are being stored
			inline bool Empty() const { return size_.load() == 0; }

			//! Returns the count of objects being stored
			inline size_t size() const { return size_; }

			//! Returns the capacity of the queue before a resize is required.
			inline size_t capacity() const { return capacity_; }
			};
		}
	}