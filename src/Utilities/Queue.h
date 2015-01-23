/*************************************************************************
Protheus Source File.
Copyright (C), Protheus Studios, 2013-2014.
-------------------------------------------------------------------------

Description:
A extern class to provide writing functions to a buffer

-------------------------------------------------------------------------
History:
- 9:01:2015: Waring J.
*************************************************************************/
#pragma once

#include <atomic>
#include "Error.h"

namespace Pro {
	namespace Util {
		/*! Thread Safe Lock Free Queue
			Queue resizes involves a lock.

			TODO:	Unsafe to push and pop during a resize, must prevent them.
		*/

		template<typename T>
		class Queue { 
			std::atomic<T*> m_queue;
			std::atomic<size_t> m_pop_pos;
			std::atomic<size_t> m_push_pos;
			std::atomic<size_t> m_size;
			std::atomic<size_t> m_capacity;  
			std::mutex resize_lock;


			inline size_t check_overflow(std::atomic<size_t>* pos) { 
				if (*pos == m_capacity - 1) {
					*pos = 0;
					return *pos;
				} 
				return (*pos)++;
			}

		public:
			Queue(const size_t size = 64) { 
				m_queue = new T[size];
				m_capacity = size;
				m_pop_pos = m_push_pos = m_size = 0;
			}
			~Queue() {
				resize_lock.lock();
				delete[] m_queue.load();
				resize_lock.unlock();
			}
			
			//! Resize is not thread safe
			inline void resize(const size_t size) {
				// TODO If a resize has been performed making it larger than the requested resize then exit after the mutex
				std::lock_guard<std::mutex> lk(resize_lock);
				 
				if (size <= m_size)
					error.reportError("Array being resized to invalid size (Size is smaller than stored objects)");
			 
				auto old_queue = m_queue.load();
				auto new_queue = new T[size]; 

				const size_t sizem = m_size;  

				if (m_push_pos < m_pop_pos) { 
					memcpy(new_queue, old_queue + m_pop_pos, sizeof(T) * (sizem - m_pop_pos)); 
					memcpy(new_queue + (sizem - m_pop_pos), old_queue, sizeof(T) * m_push_pos); 
				}
				else
					memcpy(new_queue + m_pop_pos, old_queue, sizeof(T) * (m_push_pos - m_pop_pos));

				m_capacity = size;
				m_queue.store(new_queue);
				delete[] old_queue; 
			}

			// Issue if pushing while poping with only one element
			inline void push(const T& obj) {
				if (m_size == m_capacity - 1)
					resize(static_cast<size_t>(m_capacity * 1.2f));
				auto pos = check_overflow(&m_push_pos);

				++m_size; 
				m_queue.load()[pos] = obj; 
			}

			inline T pop() {
				// Check if empty
				if (empty()) {
					error.reportError("Popped a empty queue, returned a default object.");
					return T();
				}
				// Check if at the end and set the position to 0 if true
				auto pos = check_overflow(&m_pop_pos);
				
				--m_size; 
				// Move object  
				return std::move(m_queue.load()[pos]);
			}


			inline T& peek() { return m_queue.load()[m_pop_pos]; }

			inline bool empty() const { return m_size.load() == 0; }

			inline size_t size() const { return m_size; }

		};
	}
}