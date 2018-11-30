/*************************************************************************
 Protheus Source File.
 Copyright (C), Protheus Studios, 2013-2016.
 -------------------------------------------------------------------------

 Description:

 -------------------------------------------------------------------------
 History:
 - 6:5:2015 Waring J.

 *************************************************************************/


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
		struct Node {
			Node() { _prev = _next = nullptr; }
			Node*  _prev;
			Node*  _next;
			T _ptr;
		};

		template<typename T>
		class LinkedList {

			std::mutex edit_lock;
			Node<T>* _start;
			Node<T>* _end;
			std::atomic<int> _edit_position;
			std::atomic<unsigned> _size;
			// TODO use the size to determine if empty (check if there's need for a variable)
			std::atomic<bool> _empty;

			//! Returns the node at the @index
			inline Node<T>* NodeAt(size_t index) {
				if (index > _size)
					return nullptr;
				Node<T>* ptr = _start;
				for (unsigned x = 0; x != index; ++x)
					ptr = ptr->_next;
				return ptr;
			}

			//! Removes the last remaining node without locking and returns the object ptr
			inline T RemoveLastNodeNoLock() {
				_empty.store(true);
				auto return_ptr = std::move(_start->_ptr);
				delete _start;
				_start = _end = nullptr;
				_size = 0;
				return return_ptr;
			}

			//! Removes the last node without locking and returns the object ptr
			inline T RemoveBackNoLock() {
				if (_size == 1)
					return RemoveLastNodeNoLock();
				_end = _end->_prev;
				_size.fetch_sub(1);
				auto return_ptr = std::move(_end->_next->_ptr);
				delete _end->_next;
				_end->_next = nullptr;
				return return_ptr;
			}

			//! Removes the first node without locking and returns the object ptr
			inline T RemoveFrontNoLock() {
				if (_size == 1)
					return RemoveLastNodeNoLock();
				_start = _start->_next;
				_size.fetch_sub(1);
				auto return_ptr = std::move(_start->_ptr);
				delete _start->_prev;
				_start->_prev = nullptr;
				return return_ptr;
			}

			inline bool RemoveWhere(const T* object) {
				if (_size == 0)
					return false;
				if (_size == 1 && _start->_ptr == object) {
					RemoveLastNodeNoLock();
					return true;
				}

				if (_start->_ptr == object) {
					RemoveFrontNoLock();
					return true;
				}
				if (_end->_ptr == object) {
					RemoveBackNoLock();
					return true;
				}

				auto node = _start;
				while (node != nullptr) {
					if (node->_ptr == object) {
						node->_prev->_next = node->_next;
						node->_next->_prev = node->_prev;
						_size.fetch_sub(1);
						delete node;
						return true;
					}
					node = node->_next;
				}
				return false;
			}

			//! Removes a node at @index and returns the object ptr
			inline bool RemoveAtNoLock(T* return_obj, unsigned index) {
				switch (_size.load()) {
				case 0:	return false;
				case 1:
					if (return_obj != nullptr)
						*return_obj = RemoveLastNodeNoLock();
					else
						RemoveLastNodeNoLock();	
					return true;
				}

				if (index == 0) { // Start 
					if (return_obj != nullptr)
						*return_obj = RemoveFrontNoLock();
					else
						RemoveFrontNoLock();
				}
				else if (index == _size - 1) { // End
					if (return_obj != nullptr)
						*return_obj = RemoveBackNoLock();
					else
						RemoveBackNoLock();
				}
				else {
					auto ptr = NodeAt(index);
					// Delete node 
					_edit_position.store(index);
					ptr->_next->_prev = ptr->_prev;
					ptr->_prev->_next = ptr->_next;
					_edit_position.store(--_size);
					if (return_obj != nullptr)
						*return_obj = std::move(ptr->_ptr);
					delete ptr;
				}

				return true;
			}

			//! Stores the first object in the list
			void FirstNode(T& ptr) {
				_edit_position.store(0);
				_start = _end = new Node<T>();
				_start->_prev = _start->_next = nullptr;
				_start->_ptr = ptr;
				_size++;
				_edit_position.store(_size);
				_empty.store(false);
			}

			//! Adds a object to the end of the list
			inline void Prepend(T& ptr) {
				// Prepend at the start
				auto node = new Node<T>();
				node->_ptr = ptr;
				node->_next = _start;
				node->_prev = nullptr;
				_edit_position.store(0);
				_start->_prev = node;
				_start = node;
				_size++;
				_edit_position.store(_size);
			}

			//! Adds a object to the start of the list
			inline void Append(T& ptr) {
				// Append at the end
				auto node = new Node<T>();
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
					RemoveAtNoLock(nullptr, 0);
				_size = 0;
				_start = _end = nullptr;
			}

			/*! If index is a negative value then the ptr is just appended onto the list
			*/
			void Insert(T& ptr, unsigned index) {
				// Check index is valid  
				std::lock_guard<std::mutex> lk(edit_lock);
				if (_empty)
					return FirstNode(ptr);
				// Insert into
				auto old_node = NodeAt(index);
				if (old_node == nullptr)
					return;
				if (index == 0)
					return Prepend(ptr);
				if (index == _size - 1)
					return Append(ptr);

				auto new_node = new Node<T>();
				new_node->_ptr = ptr;
				new_node->_next = old_node->_next;
				new_node->_prev = old_node;
				old_node->_next->_prev = new_node;
				old_node->_next = new_node;
				_size++;
			}

			//! Adds @ptr to the end of the list
			void PushBack(T& ptr) {
				std::lock_guard<std::mutex> lk(edit_lock);
				// Check if first element
				if (_empty.load())
					return FirstNode(ptr);
				Append(ptr);
			}

			void PushBack(T&& ptr) {
				std::lock_guard<std::mutex> lk(edit_lock);
				// Check if first element
				if (_empty.load())
					return FirstNode(ptr);
				Append(ptr);
			}

			//! Adds @ptr to the start of the list
			void PushFront(T& ptr) {
				std::lock_guard<std::mutex> lk(edit_lock);
				if (_empty.load())
					return FirstNode(ptr);
				Prepend(ptr);
			}

			//! Adds @ptr to the start of the list
			void PushFront(T&& ptr) {
				std::lock_guard<std::mutex> lk(edit_lock);
				if (_empty.load())
					return FirstNode(ptr);
				Prepend(ptr);
			}

			//! Removes an object from the array, returns true if successful and the object
			//! is returned in return_object, nullptr to ignore it's return
			inline bool Remove(unsigned index, T* return_object = nullptr) {
				std::lock_guard<std::mutex> lk(edit_lock);
				return RemoveAtNoLock(return_object, index);
			}

			//! Removes an object from the linked list 
			//! Returns true if successful
			inline bool RemoveObject(T& object) {
				std::lock_guard<std::mutex> lk(edit_lock);
				return RemoveWhere(object);
			}

			//! Removes the object at the end of the list (partner LinkedList::PushBack())
			inline T PopBack() {
				std::lock_guard<std::mutex> lk(edit_lock);
				return RemoveBackNoLock();
			}

			//! Removes the object at the start of the list (accompanies LinkedList::PushFront())
			inline T PopFront() {
				std::lock_guard<std::mutex> lk(edit_lock);
				return RemoveFrontNoLock();
			}

			//! Return the constant reference at @index
			inline const T& At(unsigned index) const {
				return NodeAt(index)->_ptr;
			}

			//! Returns the object at @index
			inline T& At(unsigned index) {
				return NodeAt(index)->_ptr;
			}

			//! Returns the count of objects stored
			inline unsigned size() const {
				return _size.load();
			}

			//! Returns true if no objects are stored
			inline bool empty() const {
				return _empty;
			}

			//! Returns the start node of the linkest list
			//! Used for efficent searching
			//! Don't modify node data from here
			inline const Node<T>* GetStartIterator() const {
				return _start;
			}

			//! Returns the start node of the linkest list
			//! Used for efficent searching
			//! Don't modify node data from here
			inline Node<T>* GetStartIterator() {
				return _start;
			}

			//! Returns the end node of the linkest list
			//! Used for efficent searching
			//! Don't modify node data from here
			inline const Node<T>* GetEndIterator() const {
				return _end;
			}

			inline Node<T>* GetEndIterator() {
				return _end;
			}

		};
	}
}