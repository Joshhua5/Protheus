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
			 
			inline Node* node_at(unsigned index) const {
				if (index > _size)
					return nullptr;
				register Node* ptr = _start;
				for (register unsigned x = 0; x < index; ++x)
					ptr = ptr->_next;
				return ptr;
			}

			inline T* remove_last_node_nl(){
				_empty.store(true); 
				auto return_ptr = _start->_ptr;
				delete _start;
				_start = _end = nullptr; 
				--_size; 
				return return_ptr;
			}

			inline T* remove_back_nl(){
				if (_size == 1)
					return remove_last_node_nl();
				_end = _end->_prev;
				_size--;
				auto return_ptr = _end->_next->_ptr;
				delete _end->_next;
				_end->_next = nullptr;
				return return_ptr;
			}

			inline T* remove_front_nl(){
				if (_size == 1)
					return remove_last_node_nl();
				_start = _start->_next;
				--_size;
				auto return_ptr = _start->_ptr;
				delete _start->_prev;
				_start->_prev = nullptr;
				return return_ptr;
			}

			inline T* remove_at_nl(unsigned index) {
				T* return_ptr;

				switch (_size){
				case 0:	return nullptr;
				case 1: return remove_last_node_nl();
				}

				if (index == 0) // Start
					return remove_front_nl();
				else if (index == _size) // End
					return remove_back_nl();
				else {
					Node* ptr = node_at(index);
					// Delete node 
					_edit_position.store(index);
					ptr->_next->_prev = ptr->_prev;
					ptr->_prev->_next = ptr->_next;
					_edit_position.store(--_size);

					return_ptr = ptr->_ptr;
					delete ptr;
				}

				return return_ptr;
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
				_size++;
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
			~LinkedList() {
				// Grab lock incase anothe thread is currently editing
				std::lock_guard<std::mutex> lk(edit_lock);
				while (!empty())
					remove_at_nl(0);
				_size = 0;
				_start = _end = nullptr;
			}

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

			void push_back(T* ptr) {
				std::lock_guard<std::mutex> lk(edit_lock);
				// Check if first element
				if (_empty.load())
					return first_node(ptr);
				_append(ptr);
			}

			void push_front(T* ptr) {
				std::lock_guard<std::mutex> lk(edit_lock);
				if (_empty.load())
					return first_node(ptr);
				_prepend(ptr);
			}

			/*! Returns the pointer that was removed from the array, does not call delete
				Unsafe to use remove(size() - 1) to remove from back in a multithread environment
				Use T* pop_back()
				*/
			inline T* remove(unsigned index) {
				std::lock_guard<std::mutex> lk(edit_lock);
				return remove_at_nl(index);
			}

			inline T* pop_back() {
				std::lock_guard<std::mutex> lk(edit_lock);
				return remove_back_nl();
			}

			inline T* pop_front() {
				std::lock_guard<std::mutex> lk(edit_lock);
				return remove_front_nl();
			}

			inline T* at(unsigned index) const {
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