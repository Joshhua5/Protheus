#pragma once
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
		And deletes the pointer when the last reference is removed,
		When trying to store objects, use a smart_ptr otherwise deconstructing will fail

		Use Pro::Deferred for controlling when arrays should be deleted
		*/
		template<typename T>
		class smart_array {
			unsigned *_references;
		public:
			T *_ptr;

			smart_array() {
				_ptr = nullptr;
				_references = nullptr;
			}
			~smart_array() {
				// Delete if a valid array is being stored
				if (_references != nullptr)
					if (--*_references == 0) {
						// did you use the correct type of smart_*?
						delete[] _ptr;
						delete _references;

						_references = nullptr;
						_ptr = nullptr;
					}
			}

			smart_array(T* ptr) {
				_ptr = ptr;
				_references = new unsigned(1);
			}

			smart_array(smart_array&& rhs) {
				_ptr = rhs._ptr;
				_references = rhs._references;
				rhs._ptr = nullptr;
				rhs._references = nullptr;
			}
			smart_array(const smart_array& rhs) {
				_ptr = rhs._ptr;
				_references = rhs._references;
				++*_references;
			}

			smart_array& operator=(smart_array&& rhs) {
				if (this == &rhs)
					return *this;
				_ptr = rhs._ptr;
				_references = rhs._references;
				rhs._ptr = nullptr;
				rhs._references = nullptr;
				return *this;
			}

			smart_array& operator=(const smart_array& rhs) {
				if (this == &rhs)
					return *this;
				_ptr = rhs._ptr;
				_references = rhs._references;
				++*_references;
				return *this;
			}

			smart_array& operator=(T* rhs) {
				// Call deconstructor on old array
				this->~smart_array();
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

			const T& operator[](const unsigned index) const {
				return _ptr[index];
			}

			bool operator==(const void* rhs) const {
				return (const void*)_ptr == rhs;
			}

			/*! Returns the current reference count */
			inline unsigned references() const {
				return *_references;
			}

			//! Removes reference to an array and returns the array.
			inline T* dereference() {
				this->~smart_array<T>();
				_references = nullptr;
				_ptr = nullptr;
			}

			inline const T* get() const { return _ptr; }
			inline T* get() { return _ptr; }

			inline bool isNull() const {
				return _ptr == nullptr;
			}
		};
	}
}