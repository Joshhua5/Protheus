#pragma once

#include <initializer_list>

namespace Pro {
	namespace Util {
		/*! BufferVector is used to store data in a dynamically expanding buffer
		*/
		template<typename T>
		class ArrayList {
			unsigned m_size;
			/*! Count of how many elements can be fit into the buffer before a resize*/
			unsigned m_reserved;
			T* m_buffer;

		public:
			ArrayList() {
				m_size = 0;
				m_reserved = 5;
				m_buffer = new T[size + m_reserved];
			}
			ArrayList(const unsigned size) {
				m_size = 0;
				m_reserved = size;
				m_buffer = new T[size + m_reserved];
			}
			ArrayList(const ArrayList& rhs) {
				m_size = rhs.m_size;
				m_reserved = rhs.m_reserved;
				m_buffer = new T[size + m_reserved];
				for (unsigned x = 0; x < m_size; ++x)
					m_buffer[x] = rhs.m_buffer[x];
			}
			ArrayList(ArrayList&& rhs) {
				m_size = rhs.m_size;
				m_reserved = rhs.m_reserved;
				m_buffer = rhs.m_buffer;
				rhs.m_buffer = nullptr;
			}

			ArrayList& operator=(const ArrayList& rhs) {
				m_size = rhs.m_size;
				m_reserved = rhs.m_reserved;
				m_buffer = new T[m_size + m_reserved]; 
				for (unsigned x = 0; x < m_size; ++x)
					m_buffer[x] = rhs.m_buffer[x];
			}

			ArrayList& operator=(ArrayList&& rhs) {
				m_size = rhs.m_size;
				m_reserved = rhs.m_reserved;
				m_buffer = rhs.m_buffer;
				rhs.m_buffer = nullptr;
			}

			~ArrayList() {
				if (m_buffer) {  
					delete[] m_buffer;
					m_buffer = nullptr;
				}
			}

			/*! Returns the element at a specified index with bounds checking*/
			inline T& at(unsigned index) const {
				if (index > m_size)
					return error.reportErrorNR("Out of bounds exception");
				return m_buffer[index];
			}

			/*! Returns the element at a specified index*/
			inline T& operator[](const unsigned index) const {
				return m_buffer[index];
			}

			/*! Adds a element to the end of the buffer */
			inline void push_back(T&& value) {
				if (m_reserved == 0)
					reserve(m_size * 1.2 + 5);
				--m_reserved;
				m_buffer[m_size++] = value;
			}

			/*! Adds a element to the end of the buffer */
			inline void push_back(const T& value) {
				if (--m_reserved == 0)
					reserve(m_size * 1.2 + 5);
				m_buffer[m_size++] = value;
			}

			/*! Returns the last element added */
			inline T& back() const {
				return m_buffer[m_size - 1];
			}

			/*! Resizes the vector and initialized the data
				no-op if the size is resize is smaller than the current size.
				TEST
			*/ 
			template<typename... T>
			void resize(const unsigned size, T&&... Args) {
				if (size < m_size)
					return;
				for (unsigned x = 0; x < m_size; ++x)
					push_back(std::move(T(constructor_args)));
			}

			/*! Reserved data to be written into, appends onto the current array
				no-op if the size is resize is smaller than the current size.
			*/
			inline void reserve(const unsigned size){
				if (size < m_size)
					return; 
				auto buffer = ::operator new[]((m_size + size) * sizeof(T));
				memcpy(buffer, m_buffer, size * sizeof(T)); 
				delete[] m_buffer;
				m_buffer = static_cast<T*>(buffer);
				m_reserved += size;
			}

			/*! Returns a pointer to the internal Buffer
				Hole are present in data if an erase has been performed since the last BufferVector::at()
				BufferVector::pack() is required if BufferVector::isPacked() is false
			*/
			inline T* data() const  {
				return m_buffer;
			}

			/*! Erase multiple elements at the same time
				Reduces the amount of pakcs that the vector must perform to 1 per batch
				TEST
			*/
			inline void erase(initializer_list<unsigned> indicies) {
				if (indicies.size == 0 && indicies.size < m_size)
					return;
				unsigned current_shift = 1;
				for (unsigned index = indicies.begin; index < m_size; indicies += current_shift) {
					if (index == indicies)
						++current_shift;
					for (unsigned shift = 0; shift < current_shift; ++shift)
						m_buffer[index + shift] = std::move(m_buffer[index + shift + current_shift]);
				}
				m_reserved += indicies.size;
			}
		};
	}
}