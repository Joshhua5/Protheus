#pragma once
#include <initializer_list>
#include <atomic>
#include <mutex>
#include "Error.h"
#include "smart_array.h"

namespace Pro {
	namespace Util {
		/*! BufferVector is used to store data in a dynamically expanding buffer
		*/

		template<typename T>
		class ArrayList {
#if ATOMIC 
			std::atomic<size_t> m_size = 0;
			std::atomic<size_t> m_reserved = 0;
			std::atomic<T*> m_buffer = nullptr;
#else
			size_t m_size = 0;
			size_t m_reserved = 0;
			/*! Count of how many elements can be fit into the buffer before a resize */
			T* m_buffer = nullptr;
#endif

			inline void copy(T* buffer, const size_t size) {
				if (is_move_constructible<T>())
					for (size_t x = 0; x < size; ++x)
						new(buffer + x) T(std::move(m_buffer[x]));
				else if (is_copy_constructible<T>())
					for (size_t x = 0; x < size; ++x)
						new(buffer + x) T(m_buffer[x]);
				else
					assert("Object T must be move or copy constructible");
			}

			inline void destroy() {
				// Deallocated initialized objects
				for (unsigned x = 0; x < m_size; ++x)
					(m_buffer + x)->~T();

				operator delete(m_buffer);
				m_buffer = nullptr;
			}

		public:
			ArrayList() {
				m_size = 0;
				m_reserved = 0;
				m_buffer = nullptr;
			}

			ArrayList(const size_t size) {
				m_size = size;
				m_reserved = 0;
				if (size == 0)
					m_buffer = nullptr;
				else {
					m_buffer = reinterpret_cast<T*>(::operator new(sizeof(T)*(m_size + m_reserved)));
					for (size_t x = 0; x < m_size; ++x)
						new (&m_buffer[x]) T();
				}
			}

			template<typename... Args>
			ArrayList(const size_t size, Args... arguments) {
				m_size = size;
				m_reserved = 0;
				if (size == 0)
					m_buffer = nullptr;
				else {
					m_buffer = reinterpret_cast<T*>(::operator new(sizeof(T)*(m_size + m_reserved)));
					// Initialize objects
					for (size_t x = 0; x < m_size; ++x)
						new (&m_buffer[x]) T(arguments...);
				}
			}

			ArrayList(const ArrayList& rhs) {
				m_size = rhs.m_size;
				m_reserved = rhs.m_reserved;
				m_buffer = reinterpret_cast<T*>(::operator new(sizeof(T)*(m_size + m_reserved)));
				for (size_t x = 0; x < m_size; ++x)
					new(&m_buffer[x]) T(rhs.m_buffer[x]);
			}

			ArrayList(ArrayList&& rhs) {
				m_size = rhs.m_size;
				m_reserved = rhs.m_reserved;
				m_buffer = rhs.m_buffer;
				rhs.m_buffer = nullptr;
			}

			ArrayList& operator=(const ArrayList& rhs) {
				if (rhs == *this)
					return;
				m_size = rhs.m_size.load();
				m_reserved = rhs.m_reserved;
				m_buffer = reinterpret_cast<T*>(::operator new(sizeof(T)*(m_size + m_reserved)));
				for (size_t x = 0; x < m_size; ++x)
					new(m_buffer[x]) T(rhs.m_buffer[x]);
				return *this;
			}

			ArrayList& operator=(ArrayList&& rhs) {
				if (rhs == *this)
					return;
				m_size = rhs.m_size.load();
				m_reserved = rhs.m_reserved;
				m_buffer = rhs.m_buffer;
				rhs.m_buffer = nullptr;
				return *this;
			}

			~ArrayList() {
				if (m_buffer != nullptr)
					destroy();
			}

			/*! Returns the element at a specified index with bounds checking*/
			inline const T& at(size_t index) const {
				if (index > m_size) {
					error.reportErrorNR("Out of bounds exception");
					return m_buffer[0];
				}
				return m_buffer[index];
			}

			inline T& at(size_t index) {
				if (index > m_size || m_buffer == nullptr) {
#if _DEBUG
					throw "Out of bounds exception.";
#endif
					error.reportErrorNR("Out of bounds exception");
					return m_buffer[0];
				}
				return m_buffer[index];
			}

			/*! Returns the element at a specified index*/
			inline const T& operator[](const size_t index) const {
				return m_buffer[index];
			}

			inline T& operator[](const size_t index) {
				return m_buffer[index];
			}

			//! Returns the size of objects used
			inline unsigned size() const {
				return m_size;
			}

			//! Returns the capacity of the ArrayList
			inline unsigned capacity() const {
				return m_size + m_reserved;
			}

			/*! Adds a element to the end of the buffer */
			inline void push_back(T&& value) {
				if (m_reserved == 0)
					resize(static_cast<size_t>(m_size * 1.2 + 5));
				--m_reserved;
				new(&m_buffer[m_size++]) T(std::move(value));

			}

			/*! Adds a element to the end of the buffer */
			inline void push_back(const T& value) {
				if (m_reserved == 0)
					resize(static_cast<size_t>(m_size * 1.2 + 5));
				--m_reserved;
				new(&m_buffer[m_size++]) T(value);

			}

			template<typename... Args>
			inline void emplace_back(Args&&... args) {
				if (m_reserved == 0)
					resize(static_cast<size_t>(m_size * 1.2 + 5));
				--m_reserved;
				new(&m_buffer[m_size++]) T(args...);
			}

			/*! Returns the last element added */
			inline const T& back() const {
				return m_buffer[m_size - 1];
			}

			inline T& back() {
				return m_buffer[m_size - 1];
			}

			inline const T& front() const {
				return m_buffer[0];
			}

			inline T& front() {
				return m_buffer[0];
			}

			//! Changes the size of the ArrayList and initialized objects
			template<typename... Args>
			void resize(const size_t size, Args... arguments) {
				T* buffer = reinterpret_cast<T*>(::operator new(sizeof(T)*(size)));
				size_t iterator_size = (size < m_size) ? size : m_size;

				if (m_buffer == nullptr) {
					m_buffer = buffer;
					m_size = 0;
					m_reserved = size;
					return;
				}

				copy(buffer, iterator_size);

				for (size_t x = iterator_size; x < size; ++x)
					new(buffer + x) T(arguments...);

				destroy();
				m_buffer = buffer;
				m_reserved = 0;
				m_size = size;
			}

			//! Increased the capacity of the ArrayList without initializing objects
			void reserve(const size_t size) {
				if (size < m_size + m_reserved)
					return;

				T* buffer = reinterpret_cast<T*>(::operator new(sizeof(T)*(size)));

				copy(buffer, m_size);

				destroy();
				m_buffer = buffer;
				m_reserved = size;
			}

			/*! Returns a pointer to the internal Buffer
				Hole are present in data if an erase has been performed since the last BufferVector::at()
				BufferVector::pack() is required if BufferVector::isPacked() is false
			*/
			inline const T* data() const {
				return m_buffer;
			}

			inline T* data() {
				return m_buffer.get();
			}

			/*! Erase multiple elements at the same time
				Reduces the amount of pakcs that the vector must perform to 1 per batch
				TEST
			*/
			inline void erase(std::initializer_list<size_t> indicies) {
				if (indicies.size() == 0 && indicies.size() < m_size)
					return;
				unsigned current_shift = 0;
				size_t* indicie = indicies.begin();
				for (unsigned index = *indicies.begin(); index < m_size; index += current_shift) {
					if (indicie != nullptr && index == *indicie) {
						++current_shift;
						++indicie;
						if (indicie == indicies.end())
							indicie = nullptr;
					}
					for (unsigned shift = 0; shift < current_shift; ++shift) {
						m_buffer[index + shift] = std::move(m_buffer[index + shift + current_shift]); 
						if (is_move_assignable<T>())
							m_buffer[index + shift] = std::move(m_buffer[index + shift + current_shift]);
						else if (is_copy_assignable<T>())
							m_buffer[index + shift] = m_buffer[index + shift + current_shift];
						else
							assert("Object T must be move or copy assignable"); 
					}
				}
				m_reserved += indicies.size();
				m_size -= indicies.size();
			}

			inline bool empty() const {
				return m_size == 0;
			}

			inline void shrink_to_fit() {
				resize(m_size);
			}

			inline T pop() {
				T object(m_buffer[--m_size]);
				// Decontruct object
				&m_buffer[m_size + 1]->~T();
				if (is_move_assignable<T>())
					return std::move(object);
				else
					return object;
			}
		};
	}
}