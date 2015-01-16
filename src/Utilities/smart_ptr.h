/*************************************************************************
Protheus Source File.
Copyright (C), Protheus Studios, 2013-2014.
-------------------------------------------------------------------------

Description:

-------------------------------------------------------------------------
History:
- 21:12:2014 Waring J.

*************************************************************************/

#pragma once
 
namespace Pro {
	namespace Util {
		/*! Keeps count of how many times a pointer is references
			And deleted the pointer when the last reference is removed,
			When trying to store arrays, use a smart_array otherwise deconstructing will fail

			Use Pro::Deferred for controlling when objects should be deleted
		*/
		template<typename T>
		class smart_ptr {
			unsigned *_references;
			T *_ptr;
		public:

			smart_ptr() {
				_ptr = nullptr;
				_references = nullptr;
			}
			~smart_ptr() {
				// Delete if a valid object is being stored
				if (_references != nullptr)
					if (--*_references == 0) {  
						// did you use the correct type of smart_*
						delete _ptr;
						delete _references;

						_references = nullptr;
						_ptr = nullptr;
					}
			}

			smart_ptr(T* ptr) {
				_ptr = ptr;
				_references = new unsigned(1);
			}

			smart_ptr(smart_ptr&& rhs) {
				_ptr = rhs._ptr;
				_references = rhs._references;
				rhs._ptr = nullptr;
				rhs._references = nullptr;
			}
			smart_ptr(const smart_ptr& rhs) {
				_ptr = rhs._ptr;
				_references = rhs._references;
				++*_references;
			}

			smart_ptr& operator=(smart_ptr&& rhs) {
				if (this == &rhs)
					return *this;
				_ptr = rhs._ptr;
				_references = rhs._references;
				rhs._ptr = nullptr;
				rhs._references = nullptr;
				return *this;
			}

			smart_ptr& operator=(const smart_ptr& rhs) {
				if (this == &rhs)
					return *this;
				_ptr = rhs._ptr;
				_references = rhs._references;
				++*_references;
				return *this;
			}

			smart_ptr& operator=(T* rhs) {
				// Call deconstructor on old object
				this->~smart_ptr(); 
				_ptr = rhs;
				_references = new unsigned(1);
				return *this;
			} 

			inline const T* operator ->() const {
				return _ptr;
			}

			inline T* operator ->() {
				return _ptr;
			}

			T& operator[](const unsigned index) {
				return _ptr[index];
			}

			const T& operator[](const unsigned index) const{
				return _ptr[index];
			}
			
			bool operator==(const void* rhs) const {
				return (const void*)_ptr == rhs;
			}

			/*! Returns the current reference count */
			inline unsigned references() const {
				return *_references;
			}
			 
			//! Removes reference to an object and returns the object
			inline void dereference() {
				if (_references != nullptr) {
					delete _references;
					_ptr = nullptr;
					_references = nullptr;
					_ptr = nullptr;
				}
			}

			inline const T* get() const { return _ptr; } 
			inline T* get() { return _ptr; }

			bool isNull() const {
				return _ptr == nullptr;
			}
		};
	}
}