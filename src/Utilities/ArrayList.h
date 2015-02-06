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
			//! Count of objects being stored
			size_t objectCount = 0;
			//! Size of headroom before more memory must be allocated
			size_t reserved = 0;
			//! Pointer to the first object
			T* objectArray = nullptr;

			inline void copy(T* buffer, const size_t size) {
				if (is_move_constructible<T>())
					for (size_t x = 0; x < size; ++x)
						new(buffer + x) T(std::move(objectArray[x]));
				else if (is_copy_constructible<T>())
					for (size_t x = 0; x < size; ++x)
						new(buffer + x) T(objectArray[x]);
				else
					assert("Object T must be move or copy constructible");
			}

			inline void copy(T* source, T* destination, const size_t offset, const size_t size) {
				if (is_move_constructible<T>())
					for (size_t i = offset; i < offset + size; ++i)
						new(destination + i) T(std::move(source[i]));
				else if (is_copy_constructible<T>())
					for (size_t i = offset; i < offset + size; ++i)
						new(destination + i) T(source[i]);
				else
					assert("Object T must be move or copy constructible");
			}

			inline void destroy() {
				// Deallocated initialized objects
				for (unsigned x = 0; x < objectCount; ++x)
					(objectArray + x)->~T();

				operator delete(objectArray);
				objectArray = nullptr;
			}

			inline T* initialize(const size_t objectCount, const size_t size) {
				if (size == 0)
					return nullptr;

				T* buffer =
					reinterpret_cast<T*>(operator new(sizeof(T) * size));
				for (size_t i = 0; i < objectCount; ++i)
					new(buffer + i) T();
				return buffer;
			}

		public:
			ArrayList() : ArrayList(0) {}

			ArrayList(const size_t size) {
				objectCount = size;
				reserved = 0;
				objectArray = initialize(size, capacity());
			}

			template<typename... Args>
			ArrayList(const size_t size, Args... arguments) {
				objectCount = size;
				reserved = 0;
				objectArray = initialize(0, capacity());
				if (objectArray != nullptr)
					for (size_t x = 0; x < objectCount; ++x)
						new (objectArray + x) T(arguments...);
			}

			ArrayList(const ArrayList& rhs) {
				objectCount = rhs.objectCount;
				reserved = rhs.reserved;
				objectArray = initialize(0, capacity());
				if (objectArray != nullptr)
					for (size_t x = 0; x < objectCount; ++x)
						new(objectArray + x) T(rhs.objectArray[x]);
			}

			ArrayList(ArrayList&& rhs) {
				objectCount = rhs.objectCount;
				reserved = rhs.reserved;
				objectArray = rhs.objectArray;
				rhs.objectArray = nullptr;
			}

			ArrayList& operator=(const ArrayList& rhs) {
				if (rhs == *this)
					return;
				objectCount = rhs.objectCount.load();
				reserved = rhs.reserved;
				objectArray = initialize(0, capacity());
				for (size_t x = 0; x < objectCount; ++x)
					new(objectArray + x) T(rhs.objectArray[x]);
				return *this;
			}

			ArrayList& operator=(ArrayList&& rhs) {
				if (rhs == *this)
					return;
				objectCount = rhs.objectCount.load();
				reserved = rhs.reserved;
				objectArray = rhs.objectArray;
				rhs.objectArray = nullptr;
				return *this;
			}

			~ArrayList() {
				if (objectArray != nullptr)
					destroy();
			}

			/*! Returns the element at a specified index with bounds checking*/
			inline const T& at(size_t index) const {
				if (index > objectCount || objectArray == nullptr) {
					error.reportErrorNR("Out of bounds exception");
					return objectArray[0];
				}
				return objectArray[index];
			}

			inline T& at(size_t index) {
				if (index > objectCount || objectArray == nullptr) {
					error.reportErrorNR("Out of bounds exception");
					return objectArray[0];
				}
				return objectArray[index];
			}

			/*! Returns the element at a specified index*/
			inline const T& operator[](const size_t index) const {
				return objectArray[index];
			}

			inline T& operator[](const size_t index) {
				return objectArray[index];
			}

			//! Returns the size of objects used
			inline unsigned size() const {
				return objectCount;
			}

			//! Returns the capacity of the ArrayList
			inline unsigned capacity() const {
				return objectCount + reserved;
			}

			/*! Adds a element to the end of the buffer */
			inline void push_back(T&& value) {
				if (reserved-- == 0)
					resize(static_cast<size_t>(objectCount * 1.2 + 5));
				new(&objectArray[objectCount++]) T(std::move(value));

			}

			/*! Adds a element to the end of the buffer */
			inline void push_back(const T& value) {
				if (reserved-- == 0)
					resize(static_cast<size_t>(objectCount * 1.2 + 5));
				new(&objectArray[objectCount++]) T(value);

			}

			template<typename... Args>
			inline void emplace_back(Args&&... args) {
				if (reserved-- == 0)
					resize(static_cast<size_t>(objectCount * 1.2 + 5));
				new(&objectArray[objectCount++]) T(args...);
			}

			/*! Returns the last element added */
			inline const T& back() const {
				return objectArray[objectCount - 1];
			}

			inline T& back() {
				return objectArray[objectCount - 1];
			}

			inline const T& front() const {
				return objectArray[0];
			}

			inline T& front() {
				return objectArray[0];
			}

			//! Changes the size of the ArrayList and initialized objects
			template<typename... Args>
			void resize(const size_t size, Args... arguments) {
				T* buffer = reinterpret_cast<T*>(::operator new(sizeof(T)*(size)));
				size_t iterator_size = (size < objectCount) ? size : objectCount;

				if (objectArray == nullptr) {
					objectArray = buffer;
					objectCount = 0;
					reserved = size;
					return;
				}

				copy(buffer, iterator_size);

				for (size_t x = iterator_size; x < size; ++x)
					new(buffer + x) T(arguments...);

				destroy();
				objectArray = buffer;
				reserved = 0;
				objectCount = size;
			}

			//! Increased the capacity of the ArrayList without initializing objects
			void reserve(const size_t size) {
				if (size < objectCount + reserved)
					return;

				T* buffer = initialize(0, size);

				copy(buffer, objectCount);

				destroy();
				objectArray = buffer;
				reserved = size - objectCount;
			}

			/*! Returns a pointer to the internal Buffer
				Hole are present in data if an erase has been performed since the last BufferVector::at()
				BufferVector::pack() is required if BufferVector::isPacked() is false
			*/
			inline const T* data() const {
				return objectArray;
			}

			inline T* data() {
				return objectArray.get();
			}

			/*! Erase multiple elements at the same time
				Reduces the amount of pakcs that the vector must perform to 1 per batch
				TEST
			*/
			inline void erase(std::initializer_list<size_t> indicies) {
				if (indicies.size() == 0 && indicies.size() < objectCount)
					return;

				// Offset due to objects already removed
				size_t offset = 0; 

				// Check how many indicies are invalid
				size_t culled = 0;
				for (size_t i = 0; i < indicies.size(); ++i)
					if (*(indicies.begin() + i) > indicies.size())
						++culled;

				// Sort indicies from lowest to highest

				size_t* sorted = new size_t[indicies.size()];
				size_t shortest = 0;
				// TODO is this defined behaviour
				// Set value to the highest possible index
				shortest -= 1;


				for (size_t y = 0; y < indicies.size(); ++y) {
					for (auto x = indicies.begin(); x != indicies.end(); ++x)
						if (*x > last_index && *x < shortest && *x < objectCount)
							shortest = *x;
					sorted[y] = shortest;
					shortest = 0 - 1;
				}

				// Move data from valid indicies 
				auto index = sorted;
				for (; index != &sorted[indicies.size() - 2 - culled]; ++index) {
					(objectArray + *index)->~T();
					// *i - offset, for the objects that were removed previously will alter the index
					// *(i + 1) - *i, to copy all objects until the next flagged object. 
					copy(objectArray + offset, objectArray, *index - offset, *(index + 1) - *index);
				}
				// Last case
				(objectArray + *index)->~T();
				// TODO test
				copy(objectArray + offset, objectArray, *index - offset, objectCount - 1 - *index);

				reserved += indicies.size();
				objectCount -= indicies.size();
			}

			inline bool empty() const {
				return objectCount == 0;
			}

			inline void shrink_to_fit() {
				resize(objectCount);
			}

			inline T pop() {
				T object(objectArray[--objectCount]);
				// Decontruct object
				(objectArray + objectCount + 1)->~T();
				return object;
			}
		};
	}
}