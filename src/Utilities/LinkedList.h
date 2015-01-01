#pragma once

#include <atomic> 
#include <mutex>

namespace Pro {
	namespace Util {
		/*! Linked List, Not say to store a new[] as the deconstructor will call delete
			Thread Safe : True
			Lock Free : False
		*/
		template<typename T>
		class LinkedList {
			struct Node {
				Node*  _prev;
				T* _ptr;
				Node*  _next;
			};
			std::mutex edit_lock;
			Node* _start;
			Node* _end;
			std::atomic<int> _edit_position;
			std::atomic<unsigned> _size;
			std::atomic<bool> _empty;

			Node* node_at(unsigned index) const {
				if (index > _size)
					return nullptr;
				register Node* ptr = _start;
				for (register unsigned x = 0; x < index; ++x)
					ptr = ptr->_next;
				return ptr;
			}

			void first_node(T* ptr) {
				_edit_position.store(0);
				_start = _end = new Node();
				_start->_prev = _start->_next = nullptr;
				_start->_ptr = ptr;
				_size++;
				_edit_position.store(_size);
				_empty.store(false);
			}

			inline void _prepend(T* ptr) {

				// Prepend at the start
				Node* node = new Node();
				node->_ptr = ptr;
				node->_next = _start;
				node->_prev = nullptr;
				_edit_position.store(0);
				_start->_prev = node;
				_start = node;
				_size++
					_edit_position.store(_size);
			}

			inline void _append(T* ptr) {
				// Append at the end
				Node* node = new Node();
				node->_ptr = ptr;
				node->_next = nullptr;
				_edit_position.store(_size - 1);
				node->_prev = _end;
				_end->_next = node;
				_end = node;
				_size++;
				_edit_position.store(_size);
			}

		public:
			LinkedList() { _start = _end = nullptr; _empty = true; _size = 0; }
			~LinkedList() {} 

			/*! If index is a negative value then the ptr is just appended onto the list
			*/
			void insert(T* ptr, unsigned index) {
				// Check index is valid  
				std::lock_guard<std::mutex> lk(edit_lock);
				if (_empty)
					return first_node(ptr);
				// Insert into
				Node* old_node = node_at(index);
				if (old_node == nullptr)
					return;
				if (index == 0)
					return _prepend(ptr);
				if (index == _size - 1)
					return _append(ptr);

				Node* new_node = new Node();
				new_node->_ptr = ptr;
				new_node->_next = old_node->_next;
				new_node->_prev = old_node;
				old_node->_next->_prev = new_node;
				old_node->_next = new_node;
				_size++; 
			}

			void append(T* ptr) { 
				std::lock_guard<std::mutex> lk(edit_lock);
				// Check if first element
				if (_empty.load())
					return first_node(ptr);
				_append(ptr); 
			}

			void prepend(T* ptr) {
				std::lock_guard(edit_lock);
				if (_empty.load())
					return first_node(ptr);
				_prepend(ptr); 
			}

			/*! Returns the pointer that was removed from the array, does not call delete
			*/
			T* remove(unsigned index) {
				T* return_ptr; 
				std::lock_guard<std::mutex> lk(edit_lock);
				 
				if (_size == 0)
					return nullptr;

				_size--;

				if (_size == 0) {
					Node* old = _start;
					return_ptr = _start->_ptr;
					_start = _end = nullptr;
					_empty.store(true);

					delete old; 
					return return_ptr;
				}

				if (index == 0) {
					_edit_position.store(0);
					// Start Node
					return_ptr = _start->_ptr;
					_start = _start->_next;
					_edit_position.store(_size);
					delete _start->_prev;
					_start->_prev = nullptr;
				}
				else if (index == _size) {
					// End node 
					return_ptr = _end->_ptr;
					auto old_end = _end;
					_end = _end->_prev;
					_end->_next = nullptr;
					delete old_end;
				}
				else {
					Node* ptr = node_at(index);
					// Delete node 
					_edit_position.store(index);
					ptr->_next->_prev = ptr->_prev;
					ptr->_prev->_next = ptr->_next;
					_edit_position.store(_size);

					return_ptr = ptr->_ptr;
					delete ptr;
				}
				 
				return return_ptr;
			}

			T* at(unsigned index) const {
				return node_at(index)->_ptr;
			}

			inline unsigned size() const {
				return _size.load();
			}

			inline bool empty() const {
				return _empty;
			}

		};
	}
}