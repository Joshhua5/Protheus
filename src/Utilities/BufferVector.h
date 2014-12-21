#pragma once

#include "Buffer.h"

#include <vector>
  
namespace Pro {
	namespace Util {
		/*! BufferVector is used to store data in a dynamically expanding buffer 
		*/
		template<typename T>
		class BufferVector {
			unsigned m_size;
			/*! Count of how many elements can be fit into the buffer before a resize*/
			unsigned m_reserved;
			Buffer m_buffer; 
			  
		public:
			BufferVector() {
				m_size = 0;
				m_reserved = 0;
				m_buffer.init(0);
			}
			BufferVector(const unsigned rhs) {
				m_size = 0;
				m_reserved = rhs;
				m_buffer.init(rhs * sizeof(T));
			}
			BufferVector(const BufferVector& rhs) {
				m_size = rhs.m_size;
				m_reserved = rhs.m_reserved;
				m_buffer = rhs.m_buffer;
			}
			BufferVector(BufferVector&& rhs) {
				m_size = rhs.m_size;
				m_reserved = rhs.m_reserved;
				m_buffer = std::move(rhs.m_buffer);
			}

			BufferVector& operator=(const BufferVector& rhs) {
				m_size = rhs.m_size;
				m_reserved = rhs.m_reserved;
				m_buffer = rhs.m_buffer;
			}

			BufferVector& operator=(BufferVector&& rhs) {
				m_size = rhs.m_size;
				m_reserved = rhs.m_reserved;
				m_buffer = std::move(rhs.m_buffer);
			}

			 
			~BufferVector() {   }

			/*! Returns the element at a specified index with bounds checking*/
			T& at(unsigned index) const {
				if (index > m_size)
					return error.reportErrorNR("Out of bounds exception");
				return *(m_buffer.data<T>() + index);
			}

			/*! Returns the element at a specified index*/
			T& operator[](const unsigned index) const {
				return *(m_buffer.data<T>() + index);
			}

			/*! Adds a element to the end of the buffer */
			void push_back(T&& value) { 
				if (m_reserved == 0)
					reserve(m_size * 1.2 + 5);
				--m_reserved;
				T* ptr = (m_buffer.data<T>() + m_size++); 
				memset(ptr, 0, sizeof(T));
				*ptr = T(value);
			}

			/*! Adds a element to the end of the buffer */
			void push_back(const T& value) {
				if (--m_reserved == 0)
					reserve(m_size * 1.2 + 5); 
				*(m_buffer.data<T>() + m_size++) = T(value);
			}

			/*! Returns the last element added */
			T& back() const {
				return m_buffer.data<T>() + m_size - 1;
			}

			/*! Resizes the vector and initialized the data
				no-op if the size is resize is smaller than the current size.
				TEST
			*/
			template<typename... T>
			void resize(const unsigned size, T... Args) {
				if (size * sizeof(T) < m_size)
					return;
				for (unsigned x = 0; x < count; ++x)
					push_back(std::move(T(constructor_args))); 
			}

			/*! Reserved data to be written into  
				no-op if the size is resize is smaller than the current size.
			*/
			void reserve(const unsigned size) {
				if (size * sizeof(T) < m_size)
					return;
				m_buffer.resize(size * sizeof(T));
				m_reserved = size - m_size;
			}

			/*! Returns a pointer to the internal Buffer
				Hole are present in data if an erase has been performed since the last BufferVector::at()
				BufferVector::pack() is required if BufferVector::isPacked() is false
			*/
			Buffer* data() const {
				return m_buffer;
			}
			  
			/*! Erase multiple elements at the same time
				Reduces the amount of pakcs that the vector must perform to 1 per batch 
				TEST
			*/
			void erase(initializer_list<unsigned> indicies) {
				if (indicies.size == 0 && indicies.size < m_size)
					return; 
				unsigned current_shift = 1;
				for (unsigned index = indicies.begin; index < m_size; indicies += current_shift) { 
					if (index == indicies)
						++current_shift;
					for (unsigned shift = 0; shift < current_shift; ++shift)
						*(m_buffer.data<T>() + index + shift) = std::move(*(m_buffer.data<T>() + index + shift + current_shift));
				}
				m_reserved += indicies.size;
			}  
		}; 
	}
}