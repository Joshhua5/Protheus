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
			// TODO use the size to determine if empty (check if there's need for a variable)
			std::atomic<bool> _empty;
			 
			inline Node* NodeAt(unsigned index) const {
				if (index > _size)
					return nullptr;
				register Node* ptr = _start;
				for (register unsigned x = 0; x < index; ++x)
					ptr = ptr->_next;
				return ptr;
			}

			inline T* RemoveLastNodeNoLock(){
				_empty.store(true); 
				auto return_ptr = _start->_ptr;
				delete _start;
				_start = _end = nullptr; 
				--_size; 
				return return_ptr;
			}

			inline T* RemoveBackNoLock(){
				if (_size == 1)
					return RemoveLastNodeNoLock();
				_end = _end->_prev;
				_size--;
				auto return_ptr = _end->_next->_ptr;
				delete _end->_next;
				_end->_next = nullptr;
				return return_ptr;
			}

			inline T* RemoveFrontNoLock(){
				if (_size == 1)
					return RemoveLastNodeNoLock();
				_start = _start->_next;
				--_size;
				auto return_ptr = _start->_ptr;
				delete _start->_prev;
				_start->_prev = nullptr;
				return return_ptr;
			}

			inline T* RemoveAtNoLock(unsigned index) {
				T* return_ptr;

				switch (_size){
				case 0:	return nullptr;
				case 1: return RemoveLastNodeNoLock();
				}

				if (index == 0) // Start
					return RemoveFrontNoLock();
				else if (index == _size) // End
					return RemoveBackNoLock();
				else {
					Node* ptr = NodeAt(index);
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

			void FirstNode(T* ptr) {
				_edit_position.store(0);
				_start = _end = new Node();
				_start->_prev = _start->_next = nullptr;
				_start->_ptr = ptr;
				_size++;
				_edit_position.store(_size);
				_empty.store(false);
			}

			inline void Prepend(T* ptr) {

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

			inline void Append(T* ptr) {
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
					RemoveAtNoLock(0);
				_size = 0;
				_start = _end = nullptr;
			}

			/*! If index is a negative value then the ptr is just appended onto the list
			*/
			void Insert(T* ptr, unsigned index) {
				// Check index is valid  
				std::lock_guard<std::mutex> lk(edit_lock);
				if (_empty)
					return FirstNode(ptr);
				// Insert into
				Node* old_node = NodeAt(index);
				if (old_node == nullptr)
					return;
				if (index == 0)
					return Prepend(ptr);
				if (index == _size - 1)
					return Append(ptr);

				Node* new_node = new Node();
				new_node->_ptr = ptr;
				new_node->_next = old_node->_next;
				new_node->_prev = old_node;
				old_node->_next->_prev = new_node;
				old_node->_next = new_node;
				_size++;
			}

			void PushBack(T* ptr) {
				std::lock_guard<std::mutex> lk(edit_lock);
				// Check if first element
				if (_empty.load())
					return FirstNode(ptr);
				Append(ptr);
			}

			void PushFront(T* ptr) {
				std::lock_guard<std::mutex> lk(edit_lock);
				if (_empty.load())
					return FirstNode(ptr);
				Prepend(ptr);
			}

			/*! Returns the pointer that was removed from the array, does not call delete
				Unsafe to use remove(size() - 1) to remove from back in a multithread environment
				Use T* pop_back()
				*/
			inline T* Remove(unsigned index) {
				std::lock_guard<std::mutex> lk(edit_lock);
				return RemoveAtNoLock(index);
			}

			inline T* PopBack() {
				std::lock_guard<std::mutex> lk(edit_lock);
				return RemoveBackNoLock();
			}

			inline T* PopFront() {
				std::lock_guard<std::mutex> lk(edit_lock);
				return RemoveFrontNoLock();
			}

			inline T* At(unsigned index) const {
				return NodeAt(index)->_ptr;
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