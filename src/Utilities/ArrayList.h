#pragma once
#include <initializer_list>
#include <atomic>
#include <mutex>
#include "Log.h"
#include "smart_array.h"

namespace Pro {
	namespace Util {
		/*! BufferVector is used to store Data in a dynamically expanding buffer
		*/

		template<typename T>
		class ArrayList {
			//! Count of objects being stored
			size_t object_count_ = 0;
			//! Size of headroom before more memory must be allocated
			size_t reserved_ = 0;
			//! Pointer to the first object
			T* object_array_ = nullptr;

			inline void Copy(T* buffer, const size_t size) {
				if (std::is_move_constructible<T>::value)
				for (size_t x = 0; x < size; ++x)
					new(buffer + x) T(std::move(object_array_[x]));
				else if (std::is_copy_constructible<T>::value)
				for (size_t x = 0; x < size; ++x)
					new(buffer + x) T(object_array_[x]);
				else
					assert("Object T must be move or copy constructible");
			}

			inline void Copy(T* source, T* destination, const size_t offset, const size_t size) {
				if (std::is_move_constructible<T>::value)
				for (size_t i = offset; i < offset + size; ++i)
					new(destination + i) T(std::move(source[i]));
				else if (std::is_copy_constructible<T>::value)
				for (size_t i = offset; i < offset + size; ++i)
					new(destination + i) T(source[i]);
				else
					assert("Object T must be move or copy constructible");
			}

			inline void Destroy() {
				// Deallocated initialized objects
				for (size_t x = 0; x < object_count_; ++x)
					(object_array_ + x)->~T();

				operator delete(object_array_);
				object_count_ = 0;
				object_array_ = nullptr;
			}

			inline T* Initialize(const size_t objectCount, const size_t size) {
				if (size == 0)
					return nullptr;

				T* buffer =
					reinterpret_cast<T*>(operator new(sizeof(T)* size));
				for (size_t i = 0; i < objectCount; ++i)
					new(buffer + i) T();
				return buffer;
			}

			template<bool initialize_all, typename... Args>
			inline void Resize(const size_t size, Args... arguments) {
				T* buffer = Initialize(0, size);
				// If true then size is being reduced
				size_t iterator_size = (size < object_count_) ? size : object_count_;

				if (object_array_ == nullptr) {
					object_array_ = buffer;
					object_count_ = 0;
					reserved_ = size;
					if (initialize_all)
					for (size_t x = iterator_size; x < size; ++x)
						new(buffer + x) T(arguments...);
					return;
				}


				Copy(buffer, iterator_size);

				if (initialize_all)
				for (size_t x = iterator_size; x < size; ++x)
					new(buffer + x) T(arguments...);

				// Destroy will set object_count_ to 0
				Destroy();
				object_array_ = buffer;

				// iterator_size is equal to the object count
				object_count_ = iterator_size;

				reserved_ = initialize_all ? 0 : size - object_count_;

			}

		public:
			ArrayList() : ArrayList(0) {}

			ArrayList(const size_t size) {
				object_count_ = size;
				reserved_ = 0;
				object_array_ = Initialize(size, capacity());
			}

			template<typename... Args>
			ArrayList(const size_t size, Args... arguments) {
				object_count_ = size;
				reserved_ = 0;
				object_array_ = Initialize(0, capacity());
				if (object_array_ != nullptr)
				for (size_t x = 0; x < object_count_; ++x)
					new (object_array_ + x) T(arguments...);
			}

			ArrayList(const ArrayList& rhs) {
				object_count_ = rhs.object_count_;
				reserved_ = rhs.reserved_;
				object_array_ = Initialize(0, capacity());
				if (object_array_ != nullptr)
				for (size_t x = 0; x < object_count_; ++x)
					new(object_array_ + x) T(rhs.object_array_[x]);
			}

			ArrayList(ArrayList&& rhs) {
				object_count_ = rhs.object_count_;
				reserved_ = rhs.reserved_;
				object_array_ = rhs.object_array_;
				rhs.object_array_ = nullptr;
			}

			ArrayList& operator=(const ArrayList& rhs) {
				if (rhs.Data() == this->Data())
					return *this;
				object_count_ = rhs.object_count_.load();
				reserved_ = rhs.reserved_;
				object_array_ = Initialize(0, capacity());
				for (size_t x = 0; x < object_count_; ++x)
					new(object_array_ + x) T(rhs.object_array_[x]);
				return *this;
			}

			ArrayList& operator=(ArrayList&& rhs) {
				if (rhs.Data() == this->Data())
					return *this;
				object_count_ = rhs.object_count_;
				reserved_ = rhs.reserved_;
				object_array_ = rhs.object_array_;
				rhs.object_array_ = nullptr;
				return *this;
			}

			~ArrayList() {
				if (object_array_ != nullptr)
					Destroy();
			}

			/*! Returns the element at a specified index with bounds checking*/
			inline const T& At(size_t index) const {
				if (index > object_count_ || object_array_ == nullptr) {
					global_log.Report<LogCode::ERROR>("Out of bounds exception", __FUNCTION__, __LINE__);
					return object_array_[0];
				}
				return object_array_[index];
			}

			inline T& At(size_t index) {
				if (index > object_count_ || object_array_ == nullptr) {
					global_log.Report<LogCode::ERROR>("Out of bounds exception", __FUNCTION__, __LINE__);
					return object_array_[0];
				}
				return object_array_[index];
			}

			/*! Returns the element at a specified index*/
			inline const T& operator[](const size_t index) const {
				return object_array_[index];
			}

			inline T& operator[](const size_t index) {
				return object_array_[index];
			}

			//! Returns the size of objects used
			inline unsigned size() const {
				return object_count_;
			}

			//! Returns the capacity of the ArrayList
			inline unsigned capacity() const {
				return object_count_ + reserved_;
			}

			/*! Adds a element to the end of the buffer */
			inline void PushBack(T&& value) {
				if (reserved_ <= 1)
					Reserve(static_cast<size_t>(object_count_ * 1.2 + 5));
				--reserved_;
				new(&object_array_[object_count_++]) T(std::move(value));

			}

			/*! Adds a element to the end of the buffer */
			inline void PushBack(const T& value) {
				if (reserved_ <= 1)
					Reserve(static_cast<size_t>(object_count_ * 1.2 + 5));
				--reserved_;
				new(&object_array_[object_count_++]) T(value);

			}

			template<typename... Args>
			inline void EmplaceBack(Args&&... args) {
				if (reserved_ <= 1)
					Reserve(static_cast<size_t>(object_count_ * 1.2 + 5));
				--reserved_;
				operator new(&object_array_[object_count_++]) T(args...);
			}

			/*! Returns the last element added */
			inline const T& Back() const {
				return object_array_[object_count_ - 1];
			}

			inline T& Back() {
				return object_array_[object_count_ - 1];
			}

			inline const T& Front() const {
				return object_array_[0];
			}

			inline T& Front() {
				return object_array_[0];
			}

			inline T Pop() {
				T object(object_array_[--object_count_]);
				// Decontruct object
				(object_array_ + object_count_ + 1)->~T();
				return object;
			}

			//! Changes the size of the ArrayList and initialized objects

			template<typename... Args>
			void Resize(const size_t size, Args... arguments) {
				Resize<true>(size, arguments...);
			}

			//! Increased the capacity of the ArrayList without initializing objects
			//! Resized will automatically occure when there's one space left.
			void Reserve(const size_t size) {
				Resize<false>(size);
			}

			/*! Returns a pointer to the internal Buffer
				Hole are present in Data if an erase has been performed since the last BufferVector::at()
				BufferVector::pack() is required if BufferVector::isPacked() is false
				*/
			inline const T* Data() const {
				return object_array_;
			}

			inline T* Data() {
				return object_array_;
			}

			/*! Erase multiple elements at the same time
				Reduces the amount of pakcs that the vector must perform to 1 per batch
				TEST
				*/
			inline void Erase(std::initializer_list<size_t> indicies) {
				if (indicies.size() == 0 && indicies.size() < object_count_)
					return;

				// Offset due to objects already removed
				size_t offset = 0;

				// Check how many indicies are invalid
				size_t culled = 0;
				for (size_t i = 0; i < indicies.size(); ++i) {
					if (*(indicies.begin() + i) > object_count_) {
						++culled;
					}
				}
				// Sort indicies from lowest to highest

				size_t* sorted = new size_t[indicies.size()];
				size_t shortest = 0;
				size_t lastIndex = 0;
				// TODO is this defined behaviour
				// Set value to the highest possible index
				shortest -= 1;


				for (size_t y = 0; y < indicies.size(); ++y) {
					for (auto x = indicies.begin(); x != indicies.end(); ++x)
						// *x == 0 for the case that lastIndex doesn't detect 0's
					if ((*x > lastIndex && *x < shortest && *x < object_count_) || *x == 0)
						shortest = *x;
					sorted[y] = shortest;
					lastIndex = shortest;
					shortest = 0 - 1;
				}

				// Move Data from valid indicies 
				auto index = sorted;
				auto end = &sorted[indicies.size() - 1] - culled;
				for (; index != end; ++index) {
					(object_array_ + *index)->~T();
					// *i - offset, for the objects that were removed previously will alter the index
					// *(i + 1) - *i, to copy all objects until the next flagged object. 
					Copy(object_array_ + offset, object_array_, *index - offset, *(index + 1) - *index);
				}
				// Last case
				(object_array_ + *index)->~T();
				// TODO test
				Copy(object_array_ + offset, object_array_, *index - offset, object_count_ - 1 - *index);

				reserved_ += (indicies.size() - culled);
				object_count_ -= (indicies.size() - culled);
			}

			inline bool Empty() const {
				return object_count_ == 0;
			}

			inline void ShrinkToFit() {
				Resize(object_count_);
			}

		};
	}
}