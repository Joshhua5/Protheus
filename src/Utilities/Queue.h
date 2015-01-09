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
		*/
		template<typename T>
		class Queue { 
			std::atomic<T*> _queue;
			std::atomic<size_t> _pop_pos;
			std::atomic<size_t> _push_pos;
			std::atomic<size_t> _size;
			std::atomic<size_t> _capacity; 

		public:
			Queue(const size_t size = 64) {
				_queue = new T[size]; 
				_capacity = size;
				_size = 0;
			}
			
			//! Resize is not thread safe
			inline void resize(const size_t size) {
				if (size <= _size)
					error.reportError("Array being resized to invalid size (Size is smaller than stored objects)");
				T* old_queue = _queue.load();
				_queue.store(new T[size]);

				if (_push_pos < _pop_pos) {
					// Two moves
					memcpy(_queue.load(), old_queue[_pop_pos], sizeof(T) * (_size - 1) - _pop_pos);
					memcpy(_queue.load()[(_size - 1) - _pop_pos], old_queue, sizeof(T) * _push_pos.load());
				}
				else
					memcpy(_queue.load()[_pop_pos.load()], old_queue, sizeof(T) * (_push_pos.load() - _pop_pos.load()));
			}

			// Issue if pushing while poping with only one element
			inline void push(const T& obj) {
				if (_size == _capacity)
					return error.reportErrorNR("Unable to add object (object dropped), Queue is full");
				
				auto pos = (_push_pos == _capacity - 1) ? _push_pos -= _capacity : _push_pos++;
				_size++; 
				_queue.load()[pos] = obj; 
			}

			inline T pop(const T& obj) {
				// Check if empty
				if (empty()) {
					error.reportError("Popped a empty queue, returned a default object.");
					return T();
				}
				// Check if at the end and set the position to 0 if true
				auto pos = (_pop_pos == _capacity - 1) ? _pop_pos -= _capacity : _pop_pos++;
				_size--;

				// Move object
				auto ret_obj = std::move(_queue.load()[pos]);
				// Deconstruct local object
				_queue.load()[pos]::~T();
				
				return ret_obj; 
			}

			inline T& peek() {
				return _queue.load()[_pop_pos];
			}

			bool empty() const {
				return size == 0;
			}

		};
	}
}