/*************************************************************************
Protheus Source File.
Copyright (C), Protheus Studios, 2013-2014.
-------------------------------------------------------------------------

Description:
A structure to hold a pointer to a buffer, also stores the size of this
buffer. The constructor will define the size in bytes and allocate the
buffer if a size is provided.

-------------------------------------------------------------------------
History:
- 1:06:2014: Waring J.
*************************************************************************/
#pragma once

#include "BufferBase.h"  

namespace Pro {
	namespace Util {
		/*! Wrapper around char arrays */
		class Buffer :
			public BufferBase
		{
		public:
			/*! If copy is false then the original pointer can't be deleted without breaking the Cbuffer */
			Buffer(const Buffer& b, bool copy = true) {
				const_cast<Buffer*>(&b)->lk.lock();
				init(b.data(), b.size(), copy);
				const_cast<Buffer*>(&b)->lk.unlock();
			}
			Buffer(void* _data, const unsigned _size, const bool copy = true) {
				if (_size == 0) {
					m_data = nullptr;
					m_size = 0;
					return;
				}

				if (copy) {
					m_data = new char[_size];
					memcpy(m_data, _data, _size);
				}
				else
					m_data = _data;

				wasCopied = copy;
				m_size = _size;
			}
			Buffer(const unsigned _size)
			{
				if (_size == 0)
					m_data = nullptr;
				else
					m_data = new char[_size];
				wasCopied = false;
				m_size = _size;
			}
			Buffer()
			{
				wasCopied = false;
				m_size = 0;
				m_data = nullptr;
			}
			Buffer(Buffer&& b) {
				m_data = b.data();
				m_size = b.size();
				wasCopied = b.wasCopied;
				b.dereference();
			}

			~Buffer()
			{
				lk.lock();
				if (wasCopied && m_size != 0)
					delete[] m_data;
				m_data = nullptr;
				lk.unlock();
			}

			inline Buffer&  operator=(Buffer&& b) {
				if (this == &b)
					return *this;
				lk.lock();
				m_data = b.data();
				m_size = b.size();
				wasCopied = b.wasCopied;
				b.dereference();
				lk.unlock();
				return *this;
			}
			inline Buffer&  operator=(const Buffer& b) { 
				if (this == &b)
					return *this;

				this->init(b.data<void>(), b.size(), true);
				return *this;
			}

			/*! Initializes the buffer with existing data
				Deleted existing data is already initialized.
			*/
			inline void  init(const void* _data, const unsigned _size, const bool copy) {
				lk.lock();
				// Check if data has been initialized
				if (wasCopied && m_data != nullptr)
					delete[] m_data;

				if (copy) {
					m_data = new char[_size];
					memcpy(m_data, _data, _size);
				}
				else
					m_data = const_cast<void*>(_data);

				wasCopied = copy;
				m_size = _size;
				lk.unlock();
			}

			inline void  init(const unsigned _size) {
				init(new char[_size], _size, false);
			}

			/*! Removed the internal reference to the heap
				Will cause memory leak if pointer hasn't been copied else were.
			*/
			inline void  dereference() {
				m_data = nullptr;
				m_size = 0;
			}

			/*! Returns a pointer to the data casted to type T */
			template<typename T>
			inline T* data() const {
				return static_cast<T*>(m_data);
			}

			/*! Returns a pointer to the data */
			inline void* data() const {
				return m_data;
			}

			/*! Returns the size of data on the heap in bytes */
			inline unsigned  size() const {
				return m_size;
			}

			/*! Returns true if size is equal to 0 */
			inline bool isEmpty() const {
				return (m_size == 0) ? true : false;
			}

			/*! Resizes the buffer to the new defined size and copies accross data
				If resized to be smaller, data is silently lost */
			inline void resize(const unsigned size) {
				lk.lock();
				const auto old_data = m_data;
				m_data = new char[size];

				if (m_size > size)
					// Reducing
					memcpy(m_data, old_data, size);
				else
					// Increasing
					memcpy(m_data, old_data, m_size);

				m_size = size;
				delete[] old_data;
				lk.unlock();
			}
			template<typename T>

			/*! Counts how many times a value occures in the buffer */
			inline unsigned count(const T& value) {
				T* buffer = data<T>();
				unsigned count = 0;
				for (size_t head = 0; head < (size() / sizeof(T)); ++head, ++buffer)
					if (*buffer == value)
						++count;
				return count;
			}

			/*! Returns a pointer with bounds checking
				Equivilant to (char*)m_data + pos
			*/
			inline void* at(const unsigned pos) const { 
				return (pos < m_size) ? reinterpret_cast<char*>(m_data) + pos : nullptr;
			}

			/*! Returns a pointer
				Equivilant to (char*)m_data + pos
			*/
			inline void* operator[](const unsigned pos) const {
				return reinterpret_cast<char*>(m_data) + pos;
			}
		};
	}
}