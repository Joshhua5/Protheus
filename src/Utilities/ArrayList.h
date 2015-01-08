#pragma once

#include <initializer_list>
#include <atomic>
#include <mutex>
#include "Error.h"

namespace Pro {
	namespace Util {
		/*! BufferVector is used to store data in a dynamically expanding buffer
		*/
		template<typename T>
		class ArrayList {
			std::atomic<unsigned> m_size;
			std::atomic<unsigned> m_erasing_at;
			/*! Count of how many elements can be fit into the buffer before a resize*/
			unsigned m_reserved;
			T* m_buffer;

			mutable std::mutex lk;

			inline void reserve_nl(unsigned size) {
				if (size < m_size)
					return; 
				T* buffer = new T[m_size + size];
				memcpy(buffer, m_buffer, m_size * sizeof(T));
				delete[] m_buffer;
				m_buffer = buffer;
				m_reserved += size; 
			}

			/*! Does not lock the structure */
			inline void push_back_nl(T&& value) {
				if (m_reserved == 0)
					reserve_nl(static_cast<unsigned>(m_size * 1.2 + 5)); 
				--m_reserved;
				m_buffer[m_size] = std::move(value);
				// Stops the m_size from being moved
				_WriteBarrier();
				m_erasing_at = ++m_size; 
			}

			inline void push_back_nl(const T& value) {
				if (m_reserved == 0)
					reserve_nl(static_cast<unsigned>(m_size * 1.2 + 5));
				--m_reserved;
				m_buffer[m_size] = value;
				// Stops the m_size from being moved
				_WriteBarrier();
				m_erasing_at = ++m_size;
			}

		public:
			ArrayList() {
				m_size = 0;
				m_reserved = 5;
				m_buffer = new T[m_reserved];
				m_erasing_at = 0;
			}
			ArrayList(const unsigned size) {
				m_size = 0;
				m_reserved = size;
				m_buffer = new T[size + m_reserved];
				m_erasing_at = 0;
			}
			ArrayList(const ArrayList& rhs) {
				rhs.lk.lock();
				lk.lock();
				m_size = rhs.m_size.load();
				m_reserved = rhs.m_reserved;
				m_buffer = new T[m_size + m_reserved];
				for (unsigned x = 0; x < m_size; ++x)
					m_buffer[x] = rhs.m_buffer[x];

				m_erasing_at = m_size.load();
				rhs.lk.unlock();
				lk.unlock();
			}
			ArrayList(ArrayList&& rhs) {
				lk.lock();
				rhs.lk.lock();
				m_size = rhs.m_size;
				m_reserved = rhs.m_reserved;
				m_buffer = rhs.m_buffer;
				rhs.m_buffer = nullptr;
				m_erasing_at = m_size;
				rhs.lk.unlock();
				lk.unlock();
			}

			ArrayList& operator=(const ArrayList& rhs) {
				lk.lock();
				rhs.lk.lock();
				m_size = rhs.m_size.load();
				m_reserved = rhs.m_reserved;
				m_buffer = new T[m_size + m_reserved];
				for (unsigned x = 0; x < m_size; ++x)
					m_buffer[x] = rhs.m_buffer[x];
				m_erasing_at = m_size.load();
				rhs.lk.unlock();
				lk.unlock();
				return *this;

			}

			ArrayList& operator=(ArrayList&& rhs) {
				lk.lock();
				rhs.lk.lock();
				m_size = rhs.m_size.load();
				m_reserved = rhs.m_reserved;
				m_buffer = rhs.m_buffer;
				rhs.m_buffer = nullptr;
				m_erasing_at = m_size.load();
				rhs.lk.unlock();
				lk.unlock();
				return *this;
			}

			~ArrayList() {
				if (m_buffer) { 
					lk.lock();
					delete[] m_buffer;
					m_buffer = nullptr; 
					lk.unlock();
				}
			}

			/*! Returns the element at a specified index with bounds checking*/
			inline T& at(unsigned index) const {
				if (index > m_size) {
					error.reportErrorNR("Out of bounds exception");
					return m_buffer[0];
				}
				// Wait until the value is finished  
				std::lock_guard<std::mutex> guard(lk);
				return m_buffer[index];
			}

			/*! Returns the element at a specified index*/
			inline T& operator[](const unsigned index) const {
				std::lock_guard<std::mutex> guard(lk);
				return m_buffer[index];
			}

			inline unsigned size() const {
				return m_size;
			}
			  
			/*! Adds a element to the end of the buffer */
			template<typename T>
			inline void push_back(T&& value) {  
				std::lock_guard<std::mutex> grd(lk);
				push_back_nl(value); 
			}

			template<typename... Args>
			inline void emplace_back(Args&&... args){
				std::lock_guard<std::mutex> grd(lk);
				push_back_nl(T(args));
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
				std::lock_guard<std::mutex> grd(lk);
				for (unsigned x = 0; x < m_size; ++x)
					push_back_nl(std::move(T(constructor_args))); 
			}

			void resize(const unsigned size) {
				if (size < m_size)
					return;
				std::lock_guard<std::mutex> grd(lk);
				for (unsigned x = 0; x < m_size; ++x)
					push_back(std::move(T())); 
			}

			/*! Reserved data to be written into, appends onto the current array
				no-op if the size is resize is smaller than the current size.
			*/
			inline void reserve(const unsigned size) {
				if (size < m_size)
					return;
				std::lock_guard<std::mutex> grd(lk);
				reserve_nl(size); 
			}

			/*! Returns a pointer to the internal Buffer
				Hole are present in data if an erase has been performed since the last BufferVector::at()
				BufferVector::pack() is required if BufferVector::isPacked() is false
			*/
			inline T* data() const {
				std::lock_guard<std::mutex> grd(lk); 
				return m_buffer; 
			}

			/*! Erase multiple elements at the same time
				Reduces the amount of pakcs that the vector must perform to 1 per batch
				TEST
			*/
			inline void erase(std::initializer_list<unsigned> indicies) {
				if (indicies.size() == 0 && indicies.size() < m_size)
					return;
				unsigned current_shift = 0;
				unsigned* indicie = (unsigned*)indicies.begin(); 
				std::lock_guard<std::mutex> grd(lk);
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
				m_erasing_at = m_size; 
			}
		};
	}
}