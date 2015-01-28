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
			unsigned *m_references;
			T *m_ptr;

			inline void destroy() {
				if (m_references != nullptr)
					if (--*m_references == 0) {
						// did you use the correct type of smart_*
						delete m_ptr;
						delete m_references;

						m_references = nullptr;
						m_ptr = nullptr;
					}
			}

		public:

			smart_ptr() {
				m_ptr = nullptr;
				m_references = nullptr;
			}
			~smart_ptr() {
				// Delete if a valid object is being stored
				destroy();  
			}

			smart_ptr(T* ptr) {
				m_ptr = ptr;
				m_references = new unsigned(1);
			}

			smart_ptr(smart_ptr&& rhs) {
				m_ptr = rhs.m_ptr;
				m_references = rhs.m_references;
				rhs.m_ptr = nullptr;
				rhs.m_references = nullptr;
			}
			smart_ptr(const smart_ptr& rhs) {
				m_ptr = rhs.m_ptr;
				m_references = rhs.m_references;
				++*m_references;
			}

			smart_ptr& operator=(smart_ptr&& rhs) {
				if (this == &rhs)
					return *this;
				m_ptr = rhs.m_ptr;
				m_references = rhs.m_references;
				rhs.m_ptr = nullptr;
				rhs.m_references = nullptr;
				return *this;
			}

			smart_ptr& operator=(const smart_ptr& rhs) {
				if (this == &rhs)
					return *this;
				m_ptr = rhs.m_ptr;
				m_references = rhs.m_references;
				++*m_references;
				return *this;
			}

			smart_ptr& operator=(T* rhs) {
				// Call deconstructor on old object
				destroy();
				m_ptr = rhs;
				m_references = new unsigned(1);
				return *this;
			}

			inline const T* operator ->() const {
				return m_ptr;
			}

			inline T* operator ->() {
				return m_ptr;
			}

			T& operator[](const unsigned index) {
				return m_ptr[index];
			}

			const T& operator[](const unsigned index) const {
				return m_ptr[index];
			}

			bool operator==(const void* rhs) const {
				return (const void*)m_ptr == rhs;
			}

			/*! Returns the current reference count */
			inline unsigned references() const {
				if (m_references == nullptr)
					return 0;
				return *m_references;
			}

			//! Removes reference to an object and returns the object
			inline void dereference() {
				destroy();
			}

			inline const T* get() const { return m_ptr; }
			inline T* get() { return m_ptr; }

			bool isNull() const {
				return m_ptr == nullptr;
			}
		};
	}
}