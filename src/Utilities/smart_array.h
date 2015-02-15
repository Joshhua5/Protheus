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
			unsigned *references_;
			T *ptr_;


			inline void Destroy(const bool clean = true) {
				// Delete if a valid array is being stored
				if (references_ != nullptr)
					if (--*references_ == 0) {
						// did you use the correct type of smart_*?
						if(clean)
							delete[] ptr_;
						delete references_;

						references_ = nullptr;
						ptr_ = nullptr;
					}
			}

		public:
			smart_array() {
				ptr_ = nullptr;
				references_ = nullptr;
			}
			~smart_array() {
				Destroy();
			}

			smart_array(T* ptr) {
				if (ptr_ == nullptr)
					references_ = nullptr;
				else
					references_ = new unsigned(1); 
				ptr_ = ptr;
			}

			smart_array(smart_array&& rhs) {
				ptr_ = rhs.ptr_;
				references_ = rhs.references_;
				rhs.ptr_ = nullptr;
				rhs.references_ = nullptr;
			}
			smart_array(const smart_array& rhs) {
				ptr_ = rhs.ptr_;
				references_ = rhs.references_;
				++*references_;
			}

			smart_array& operator=(smart_array&& rhs) {
				if (this == &rhs)
					return *this;
				ptr_ = rhs.ptr_;
				references_ = rhs.references_;
				rhs.ptr_ = nullptr;
				rhs.references_ = nullptr;
				return *this;
			}

			smart_array& operator=(const smart_array& rhs) {
				if (this == &rhs)
					return *this;
				ptr_ = rhs.ptr_;
				references_ = rhs.references_;
				++*references_;
				return *this;
			}

			smart_array& operator=(T* rhs) {
				// Call deconstructor on old array
				
				Destroy();
				ptr_ = rhs;
				if (rhs == nullptr)
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
				return *references_;
			}

			//! Removes reference to an array
			inline void Dereference(const bool clean = true) {
				Destroy(clean);
			}
			  
			inline const T* get() const { return ptr_; }
			inline T* get() { return ptr_; }

			inline bool IsNull() const {
				return ptr_ == nullptr;
			}
		};
	}
}