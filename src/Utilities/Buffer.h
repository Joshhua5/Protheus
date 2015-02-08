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
				const_cast<Buffer*>(&b)->lock_.lock();
				Init(b.data(), b.size(), copy);
				const_cast<Buffer*>(&b)->lock_.unlock();
			}
			Buffer(void* _data, const unsigned _size, const bool copy = true) {
				if (_size == 0) {
					data_ = nullptr;
					size_ = 0;
					return;
				}

				if (copy) {
					data_ = new char[_size];
					memcpy(data_, _data, _size);
				}
				else
					data_ = _data;

				was_copied_ = copy;
				size_ = _size;
			}
			Buffer(const unsigned _size)
			{
				if (_size == 0)
					data_ = nullptr;
				else
					data_ = new char[_size];
				was_copied_ = false;
				size_ = _size;
			}
			Buffer()
			{
				was_copied_ = false;
				size_ = 0;
				data_ = nullptr;
			}
			Buffer(Buffer&& b) {
				data_ = b.data();
				size_ = b.size();
				was_copied_ = b.was_copied_;
				b.Dereference();
			}

			~Buffer()
			{
				lock_.lock();
				if (was_copied_ && size_ != 0)
					delete[] data_;
				data_ = nullptr;
				lock_.unlock();
			}

			inline Buffer&  operator=(Buffer&& b) {
				if (this == &b)
					return *this;
				lock_.lock();
				data_ = b.data();
				size_ = b.size();
				was_copied_ = b.was_copied_;
				b.Dereference();
				lock_.unlock();
				return *this;
			}
			inline Buffer&  operator=(const Buffer& b) { 
				if (this == &b)
					return *this;

				this->Init(b.data<void>(), b.size(), true);
				return *this;
			}

			/*! Initializes the buffer with existing data
				Deleted existing data is already initialized.
			*/
			inline void  Init(const void* _data, const unsigned _size, const bool copy) {
				lock_.lock();
				// Check if data has been initialized
				if (was_copied_ && data_ != nullptr)
					delete[] data_;

				if (copy) {
					data_ = new char[_size];
					memcpy(data_, _data, _size);
				}
				else
					data_ = const_cast<void*>(_data);

				was_copied_ = copy;
				size_ = _size;
				lock_.unlock();
			}

			inline void  Init(const unsigned _size) {
				Init(new char[_size], _size, false);
			}

			/*! Removed the internal reference to the heap
				Will cause memory leak if pointer hasn't been copied else were.
			*/
			inline void Dereference() {
				data_ = nullptr;
				size_ = 0;
			}
			 
			/*! Returns true if size is equal to 0 */
			inline bool Empty() const {
				return (size_ == 0) ? true : false;
			}

			/*! Resizes the buffer to the new defined size and copies accross data
				If resized to be smaller, data is silently lost */
			inline void Resize(const unsigned size) {
				lock_.lock();
				const auto old_data = data_;
				data_ = new char[size];

				if (size_ > size)
					// Reducing
					memcpy(data_, old_data, size);
				else
					// Increasing
					memcpy(data_, old_data, size_);

				size_ = size;
				delete[] old_data;
				lock_.unlock();
			} 

			/*! Counts how many times a value occures in the buffer */
			template<typename T>
			inline unsigned Count(const T& value) {
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
			inline void* At(const unsigned pos) const { 
				return (pos < size_) ? reinterpret_cast<char*>(data_) + pos : nullptr;
			}

			/*! Returns a pointer
				Equivilant to (char*)m_data + pos
			*/
			inline void* operator[](const unsigned pos) const {
				return reinterpret_cast<char*>(data_) + pos;
			}
		};
	}
}