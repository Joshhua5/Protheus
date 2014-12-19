#pragma once


namespace Pro {

	template<typename T>
	class smart_pointer {
		unsigned *_references;
	public:
		T *_ptr;

		smart_pointer() {
			_ptr = nullptr;
			_references = nullptr;
		}
		~smart_pointer() {
			if (_references != nullptr)
				if (--*_references == 0) {
					delete _ptr;
					delete _references;
				}
		}

		smart_pointer(T* ptr) {
			_ptr = ptr;
			_references = new unsigned(1);
		}

		smart_pointer(smart_pointer&& rhs) {
			_ptr = rhs._ptr;
			_references = rhs._references;
			rhs._ptr = nullptr;
			rhs._references = nullptr;
		}
		smart_pointer(const smart_pointer& rhs) {
			_ptr = rhs._ptr;
			_references = rhs._references;
			++*_references;
		}

		smart_pointer& operator=(smart_pointer&& rhs) {
			_ptr = rhs._ptr;
			_references = rhs._references;
			rhs._ptr = nullptr;
			rhs._references = nullptr;
			return *this;
		}
		bool operator==(void* rhs) {
			return (void*)_ptr == rhs;
		}
		smart_pointer& operator=(const smart_pointer& rhs) {
			_ptr = rhs._ptr;
			_references = rhs._references;
			++*_references;
			return *this;
		}
		smart_pointer& operator=(T* rhs) {
			if (_references != nullptr)
				if (--*_references == 0) {
					delete _ptr;
					delete _references;
				}

			_ptr = rhs;
			_references = new unsigned(1);
			return *this;
		}
		T* operator ->() {
			return _ptr;
		}

		inline unsigned references() const {
			return *_references;
		}

		// removed reference to an object and doesn't delete the instance unless the reference count is 1
		inline void dereference() { 
			if (_references != nullptr) {
				delete _references;
				_ptr = nullptr;
				_references = nullptr;
				_ptr = nullptr;
			}
		}
	};
}