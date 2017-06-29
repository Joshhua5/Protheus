/*************************************************************************
Protheus Source File.
Copyright (C), Protheus Studios, 2013-2016.
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
				data_ = nullptr;
				size_ = 0;
				was_copied_ = false; 
				Init(b.data(), b.size(), copy); 
			}
			Buffer(void* _data, const size_t _size, const bool copy = true) {
				if (_size == 0) {
					data_ = nullptr;
					size_ = 0;
					return;
				}

				if (copy) {
					data_ = new char[_size];
					std::memcpy(data_, _data, _size);
				}
				else
					data_ = _data;

				was_copied_ = copy;
				size_ = _size;
			}
            
			Buffer(const size_t _size)
			{
				if (_size == 0)
					data_ = nullptr;
				else
					data_ = new char[_size];
				was_copied_ = true;
				size_ = _size;
			}
			Buffer()
			{
				was_copied_ = true;
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
				if (was_copied_ && size_ != 0)
					delete[] static_cast<char*>(data_);
				data_ = nullptr; 
			}

			inline Buffer&  operator=(Buffer&& b) {
				if (this == &b)
					return *this; 
				data_ = b.data();
				size_ = b.size();
				was_copied_ = b.was_copied_;
				b.Dereference(); 
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
			inline void  Init(const void* _data, const size_t _size, const bool copy) { 
				// Check if data has been initialized
                if (was_copied_ && data_ != nullptr)
                    delete[] static_cast<char*>(data_);
				if (copy) {
					data_ = new char[_size];
					std::memcpy(data_, _data, _size);
				}
				else
					data_ = const_cast<void*>(_data);

				was_copied_ = copy;
				size_ = _size; 
			}

			inline void  Init(const size_t _size) {
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
			inline void Resize(const size_t size) {
                if(size_ == 0){
                    Init(size);
                    return;
                } 
				if (size_ != size) {
					const auto old_data = data_;
					data_ = new char[size];

					// size is the new size and size_ is the original size
					std::memcpy(data_, old_data, (size_ > size) ? size : size_);

					size_ = size;
					delete[] static_cast<char*>(old_data);
				}
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
			inline void* At(const size_t pos) const {
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