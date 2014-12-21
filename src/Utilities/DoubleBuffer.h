/*************************************************************************
Protheus Source File.
Copyright (C), Protheus Studios, 2013-2014.
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
			T* front;
			T* back;
		public:
			/*! Will call the default constructor on T */
			DoubleBuffer() {
				front = new T();
				back = new T();
			}
			DoubleBuffer(T& value) {
				*front = *back = value;
			}
			DoubleBuffer(const DoubleBuffer&) {
				*this->back = *rhs.back;
				*this->front = *rhs.front;
			}
			DoubleBuffer(DoubleBuffer&&) {
				this->back = rhs.back;
				this->front = rhs.front;
				rhs.back = rhs.front = nullptr;
			}
			DoubleBuffer& operator=(const DoubleBuffer& rhs) {
				*this->back = *rhs.back;
				*this->front = *rhs.front;
			}
			DoubleBuffer& operator=(DoubleBuffer&& rhs) {
				this->back = rhs.back;
				this->front = rhs.front;
				rhs.back = rhs.front = nullptr;
			}
			~DoubleBuffer() {
				delete back;
				delete front;
			}

			/*! Returns the front value */
			T* getCurrentValue() const {
				return front;
			}

			/*! Returns the back value*/
			T* getBackValue() const {
				return back;
			}

			/*! Copied the front buffer to the back and clears the old value of the back buffer*/
			void swap() {
				*back = *front;
			}
		};
	}
}
