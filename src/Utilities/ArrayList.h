#pragma once

#include <initializer_list>
#include <atomic>
#include <mutex>
#include "Error.h"
#include "smart_array.h"

//#include <vector>

namespace Pro {
	namespace Util {
		/*! BufferVector is used to store data in a dynamically expanding buffer
		*/

		//#define ArrayList std::vector

		template<typename T>
		class ArrayList {
			std::atomic<size_t> m_size = 0;
			std::atomic<size_t> m_erasing_at = 0;
			/*! Count of how many elements can be fit into the buffer before a resize */
			size_t m_reserved = 0;
			smart_array<T> m_buffer = nullptr;

				mutable std::mutex arraylist_lock;

			inline void reserve_nl(const size_t size) {
				if (size < m_size)
					return;

				if (m_buffer == nullptr) {
					m_buffer = new T[size];
					m_size = 0;
					m_reserved = size;
					return;
				}

				T* buffer = new T[m_size + size];
				for (unsigned x = 0; x < m_size; ++x)
					// Call object moves for a safe resize
					buffer[x] = std::move(m_buffer.get()[x]);
				m_buffer = buffer;
				m_reserved += size;
			}

			/*! Does not lock the structure */
			inline void push_back_nl(T&& value) {
				if (m_reserved == 0)
					reserve_nl(static_cast<size_t>(m_size * 1.2 + 5));
				--m_reserved;
				m_buffer[m_size] = std::move(value);
				// Stops the m_size from being moved
				_WriteBarrier();
				m_erasing_at = ++m_size;
			}

			inline void push_back_nl(const T& value) {
				if (m_reserved == 0)
					reserve_nl(static_cast<size_t>(m_size * 1.2 + 5));
				--m_reserved;
				m_buffer[m_size] = value;
				// Stops the m_size from being moved
				_WriteBarrier();
				m_erasing_at = ++m_size;
			}

		public:
			ArrayList() {
				m_size = 0;
				m_reserved = 0;
				m_buffer = nullptr;
				m_erasing_at = 0;
			}
			ArrayList(const size_t size) {
				m_size = 0;
				m_reserved = size;
				m_buffer = (size == 0) ? nullptr : new T[m_size + m_reserved];
				m_erasing_at = 0;
			}
			ArrayList(const ArrayList& rhs) {
				rhs.arraylist_lock.lock();
				arraylist_lock.lock();
				m_size = rhs.m_size.load();
				m_reserved = rhs.m_reserved;
				m_buffer = new T[m_size + m_reserved];
				for (size_t x = 0; x < m_size; ++x)
					m_buffer[x] = rhs.m_buffer[x];

				m_erasing_at = m_size.load();
				rhs.arraylist_lock.unlock();
				arraylist_lock.unlock();
			}
			ArrayList(ArrayList&& rhs) {
				arraylist_lock.lock();
				rhs.arraylist_lock.lock();
				m_size = rhs.m_size.load();
				m_reserved = rhs.m_reserved;
				m_buffer = rhs.m_buffer;
				rhs.m_buffer = nullptr;
				m_erasing_at = m_size.load();
				rhs.arraylist_lock.unlock();
				arraylist_lock.unlock();
			}

			ArrayList& operator=(const ArrayList& rhs) {
				arraylist_lock.lock();
				rhs.arraylist_lock.lock();
				m_size = rhs.m_size.load();
				m_reserved = rhs.m_reserved;
				m_buffer = new T[m_size + m_reserved];
				for (size_t x = 0; x < m_size; ++x)
					m_buffer[x] = rhs.m_buffer[x];
				m_erasing_at = m_size.load();
				rhs.arraylist_lock.unlock();
				arraylist_lock.unlock();
				return *this;
			}

			ArrayList& operator=(ArrayList&& rhs) {
				arraylist_lock.lock();
				rhs.arraylist_lock.lock();
				m_size = rhs.m_size.load();
				m_reserved = rhs.m_reserved;
				m_buffer = rhs.m_buffer;
				rhs.m_buffer = nullptr;
				m_erasing_at = m_size.load();
				rhs.arraylist_lock.unlock();
				arraylist_lock.unlock();
				return *this;
			}

			~ArrayList() {
				if (!m_buffer.isNull()) {
					arraylist_lock.lock();
					m_buffer = nullptr;
					arraylist_lock.unlock();
				}
			}

			/*! Returns the element at a specified index with bounds checking*/
			inline const T& at(size_t index) const {
				if (index > m_size) {
					error.reportErrorNR("Out of bounds exception");
					return m_buffer[0];
				}
				// Wait until the value is finished  
				std::lock_guard<std::mutex> guard(arraylist_lock);
				return m_buffer[index];
			}

			inline T& at(size_t index) {
				if (index > m_size) {
					error.reportErrorNR("Out of bounds exception");
					return m_buffer[0];
				}
				// Wait until the value is finished  
				std::lock_guard<std::mutex> guard(arraylist_lock);
				return m_buffer[index];
			}

			/*! Returns the element at a specified index*/
			inline const T& operator[](const size_t index) const {
				std::lock_guard<std::mutex> guard(arraylist_lock);
				return m_buffer[index];
			}

			inline T& operator[](const size_t index) {
				std::lock_guard<std::mutex> guard(arraylist_lock);
				return m_buffer[index];
			}

			//! Returns the size of used objects
			inline unsigned count() const {
				return m_size;
			}

			//! Returns the size of objects used
			inline unsigned size() const {
				return m_size + m_reserved;
			}

			//! Returns the count of objects reserved
			inline unsigned reserved() const {
				return m_reserved;
			}

			/*! Adds a element to the end of the buffer */
			inline void push_back(T&& value) {
				std::lock_guard<std::mutex> grd(arraylist_lock);
				push_back_nl(std::move(value));
			}

			/*! Adds a element to the end of the buffer */
			inline void push_back(const T& value) {
				std::lock_guard<std::mutex> grd(arraylist_lock);
				push_back_nl(value);
			}

			template<typename... Args>
			inline void emplace_back(Args&&... args) {
				std::lock_guard<std::mutex> grd(arraylist_lock);
				push_back_nl(T(args...));
			}

			/*! Returns the last element added */
			inline const T& back() const {
				return m_buffer[m_size - 1];
			}

			/*! Resizes the vector and initialized the data
				no-op if the size is resize is smaller than the current size.
				TEST
			*/
			template<typename... T>
			void resize(const size_t size, T&&... Args) {
				if (size < m_size)
					return;
				std::lock_guard<std::mutex> grd(arraylist_lock);
				for (unsigned x = 0; x < m_size; ++x)
					push_back_nl(std::move(T(constructor_args)));
			}

			void resize(const size_t size) {
				if (size < m_size)
					return;
				std::lock_guard<std::mutex> grd(arraylist_lock);
				for (unsigned x = 0; x < m_size; ++x)
					push_back(std::move(T()));
			}

			/*! Reserved data to be written into, appends onto the current array
				no-op if the size is resize is smaller than the current size.
			*/
			inline void reserve(const size_t size) {
				if (size < m_size)
					return;
				std::lock_guard<std::mutex> grd(arraylist_lock);
				reserve_nl(size);
			}

			/*! Returns a pointer to the internal Buffer
				Hole are present in data if an erase has been performed since the last BufferVector::at()
				BufferVector::pack() is required if BufferVector::isPacked() is false
			*/
			inline const T* data() const {
				std::lock_guard<std::mutex> grd(arraylist_lock);
				return m_buffer;
			}

			inline T* data() {
				std::lock_guard<std::mutex> grd(arraylist_lock);
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
				unsigned* indicie = (unsigned*)indicies.begin();
				std::lock_guard<std::mutex> grd(arraylist_lock);
				m_erasing_at = *indicies.begin();
				for (unsigned index = *indicies.begin(); index < m_size; index += current_shift) {
					if (indicie != nullptr && index == *indicie) {
						++current_shift;
						++indicie;
						if (indicie == indicies.end())
							indicie = nullptr;
					}
					for (unsigned shift = 0; shift < current_shift; ++shift)
						m_buffer[index + shift] = std::move(m_buffer[index + shift + current_shift]);
				}
				m_reserved += indicies.size();
				m_size -= indicies.size();
				m_erasing_at = m_size.load();
			}
		};
	}
}