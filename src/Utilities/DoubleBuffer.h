
#pragma once
#include <vector>

namespace Pro {
	using namespace std;
	 
	template <typename T>
	class DoubleBuffer {
		// if the set is true then the back buffer is at the front
		// otherwise the back buffer is
		T* current;
		T* old; 
	public: 
		DoubleBuffer() { 
			current = new T();
			old = new T();
		}
		DoubleBuffer(T& value) {
			*current = *old = value;
		}
		DoubleBuffer(const DoubleBuffer&) {
			*this->old = *rhs.old;
			*this->current = *rhs.current;
		}
		DoubleBuffer(DoubleBuffer&&) {
			this->old = rhs.old;
			this->current = rhs.current;
			rhs.old = rhs.current = nullptr;
		}
		DoubleBuffer& operator=(const DoubleBuffer& rhs) {
			*this->old = *rhs.old;
			*this->current = *rhs.current;
		}
		DoubleBuffer& operator=(DoubleBuffer&& rhs) {
			this->old = rhs.old;
			this->current = rhs.current;
			rhs.old = rhs.current = nullptr;
		}
		~DoubleBuffer() {
			delete old;
			delete current;
		}
		 
		T* getCurrentValue() {
			return current;
		}

		T* getOldValue() {
			return old;
		}
		
		void swap() {
			*current = *old;
		}

	};
}

