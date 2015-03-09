/*************************************************************************
Protheus Source File.
Copyright (C), Protheus Studios, 2013-2015.
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
			unsigned *references_;
			T *ptr_;

			inline void Destroy(const bool clean = true) {
				if (references_ != nullptr)
					if (--*references_ == 0) {
						// did you use the correct type of smart_*
						if(clean)
							delete ptr_;
						delete references_;

						references_ = nullptr;
						ptr_ = nullptr;
					}
			}

		public:

			smart_ptr() {
				ptr_ = nullptr;
				references_ = nullptr;
			}
			~smart_ptr() {
				// Delete if a valid object is being stored
				Destroy();
			}

			smart_ptr(T* ptr) {
				ptr_ = ptr;
				if (ptr_ == nullptr)
					references_ = nullptr;
				else
					references_ = new unsigned(1);
			}

			smart_ptr(smart_ptr&& rhs) { 
				if (rhs.IsNull()){
					// Check for a null ptr
					references_ = nullptr;
					ptr_ = nullptr;
					return;
				}
				ptr_ = rhs.ptr_;
				references_ = rhs.references_;
				rhs.ptr_ = nullptr;
				rhs.references_ = nullptr;
			}
			smart_ptr(const smart_ptr& rhs) {
				if (rhs.IsNull()){
					// Check for a null ptr
					references_ = nullptr;
					ptr_ = nullptr; 
					return;
				}
				ptr_ = rhs.ptr_;
				references_ = rhs.references_;
				++*references_;
			}

			smart_ptr& operator=(smart_ptr&& rhs) {
				if (this == &rhs || rhs.IsNull())
					return *this;
				ptr_ = rhs.ptr_;
				references_ = rhs.references_;
				rhs.ptr_ = nullptr;
				rhs.references_ = nullptr;
				return *this;
			}

			smart_ptr& operator=(const smart_ptr& rhs) {
				if (this == &rhs || rhs.IsNull())
					return *this;
				ptr_ = rhs.ptr_;
				references_ = rhs.references_;
				++*references_;
				return *this;
			}

			smart_ptr& operator=(T* rhs) {
				// Call deconstructor on old object
				Destroy();
				ptr_ = rhs;
				if (ptr_ == nullptr)
					references_ = nullptr;
				else
					references_ = new unsigned(1);
				return *this;
			}

			inline const T* operator ->() const {
				return ptr_;
			}

			inline T* operator ->() {
				return ptr_;
			}

			T& operator[](const unsigned index) {
				return ptr_[index];
			}

			const T& operator[](const unsigned index) const {
				return ptr_[index];
			}

			bool operator==(const void* rhs) const {
				return (const void*)ptr_ == rhs;
			}

			/*! Returns the current reference count */
			inline unsigned references() const {
				if (references_ == nullptr)
					return 0;
				return *references_;
			}

			//! Removes reference to an object and the object unless 
			//! Clean is set to false
			inline void Dereference(bool clean = true) {
				Destroy(clean);
			} 

			//! Returns the stored pointer as a const
			inline const T* get() const { return ptr_; }

			//! Returns the stored pointer
			inline T* get() { return ptr_; }

			//! Returns true if the pointer is null
			bool IsNull() const {
				return ptr_ == nullptr;
			}
		};
	}
}