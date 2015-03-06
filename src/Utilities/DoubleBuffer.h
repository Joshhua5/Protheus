/*************************************************************************
Protheus Source File.
Copyright (C), Protheus Studios, 2013-2015.
-------------------------------------------------------------------------

Description:

-------------------------------------------------------------------------
History:
- 21:12:2014 Waring J.

*************************************************************************/
#pragma once
#include <vector>

namespace Pro {
	namespace Util {
		using namespace std;

		/*! Allows data to be updated while keeping track of it's original state before the last swap. */
		template <typename T>
		class DoubleBuffer {
			// if the set is true then the back buffer is at the front
			// otherwise the back buffer is
			T* front_;
			T* back_;
		public:
			/*! Will call the default constructor on T */
			DoubleBuffer() {
				front_ = operator new(sizeof(T));
				back_ =  operator new(sizeof(T));
			}
			DoubleBuffer(T value) {
				operator new(front_) T(value);
				operator new(back_) T(value); 
			}
			DoubleBuffer(const DoubleBuffer&) {
				*this->back_ = *rhs.back;
				*this->front_ = *rhs.front;
			}
			DoubleBuffer(DoubleBuffer&&) {
				this->back_ = rhs.back;
				this->front_ = rhs.front;
				rhs.back = rhs.front = nullptr;
			}
			DoubleBuffer& operator=(const DoubleBuffer& rhs) {
				*this->back_ = *rhs.back_;
				*this->front_ = *rhs.front_;
			}
			DoubleBuffer& operator=(DoubleBuffer&& rhs) {
				this->back_ = rhs.back_;
				this->front_ = rhs.front_;
				rhs.back_ = rhs.front_ = nullptr;
			}
			~DoubleBuffer() {
				back_->~T();
				front_->~T();
				operator delete (back_);
				operator delete (front_);
			}

			/*! Returns the front value */
			const T* front() const {
				return front_;
			}

			/*! Returns the back value*/
			const T* back() const {
				return back_;
			}
			/*! Returns the front value */
			 T* front()  {
				return front_;
			}

			/*! Returns the back value*/
			 T* back()  {
				return back_;
			}

			/*! Copied the front buffer to the back and clears the old value of the back buffer*/
			void Swap() {
				*back_ = *front_;
			}
		};
	}
}
