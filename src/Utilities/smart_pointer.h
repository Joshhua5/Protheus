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
			And deleted the pointer when the last reference is removed

			Use Pro::Deferred for controlling when objects should be deleted
		*/
		template<typename T>
		class smart_pointer {
			unsigned *_references;
		public:
			T *_ptr;

			smart_pointer() {
				_ptr = nullptr;
				_references = nullptr;
			}
			~smart_pointer() {
				if (_references != nullptr)
					if (--*_references == 0) { 
						delete _ptr;
						delete _references;

						_references = nullptr;
						_ptr = nullptr;
					}
			}

			smart_pointer(T* ptr) {
				_ptr = ptr;
				_references = new unsigned(1);
			}

			smart_pointer(smart_pointer&& rhs) {
				_ptr = rhs._ptr;
				_references = rhs._references;
				rhs._ptr = nullptr;
				rhs._references = nullptr;
			}
			smart_pointer(const smart_pointer& rhs) {
				_ptr = rhs._ptr;
				_references = rhs._references;
				++*_references;
			}

			smart_pointer& operator=(smart_pointer&& rhs) {
				if (this == &rhs)
					return *this;
				_ptr = rhs._ptr;
				_references = rhs._references;
				rhs._ptr = nullptr;
				rhs._references = nullptr;
				return *this;
			}

			smart_pointer& operator=(const smart_pointer& rhs) {
				if (this == &rhs)
					return *this;
				_ptr = rhs._ptr;
				_references = rhs._references;
				++*_references;
				return *this;
			}

			smart_pointer& operator=(T* rhs) {
				if (_references != nullptr)
					if (--*_references == 0) {
						delete _ptr;
						delete _references;
					}
				_ptr = rhs;
				_references = new unsigned(1);
				return *this;
			} 

			inline T* operator ->() const {
				return _ptr;
			}

			T& operator[](const unsigned index) const{
				return _ptr[index];
			}
			
			bool operator==(void* rhs) const {
				return (void*)_ptr == rhs;
			}
			/*! Returns the current reference count */
			inline unsigned references() const {
				return *_references;
			}

			// removed reference to an object and doesn't delete the instance unless the reference count is 1
			inline void dereference() {
				if (_references != nullptr) {
					delete _references;
					_ptr = nullptr;
					_references = nullptr;
					_ptr = nullptr;
				}
			}
		};
	}
}