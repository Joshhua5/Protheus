#pragma once

struct Counter {
	int constructor = 0;
	int copy_constructor  = 0;
	int move_constructor  = 0;
	int destructor  = 0;
	int copy  = 0;
	int move  = 0;
	int multiplication_assignment = 0;
	void reset() {
		constructor  = 0;
		copy_constructor  = 0;
		move_constructor  = 0;
		destructor  = 0;
		copy  = 0;
		move  = 0;
		multiplication_assignment = 0;
	}
}; 

class TestCls {
private:
	int *m_value = nullptr;
	Counter* call_count;
public: 
	TestCls() {
		m_value = new int(0);
	}
	TestCls(Counter* counter) {
		m_value = new int(0);
		call_count = counter;
		++(call_count->constructor);
	}
	TestCls(Counter* counter, int value) {
		m_value = new int(value);
		call_count = counter;
		++(call_count->constructor);
	}
	TestCls(const TestCls &other) {
		m_value = new int(*other.m_value);
		call_count = other.call_count;
		++(call_count->constructor);
		++(call_count->copy_constructor);
	}
	TestCls(TestCls &&other) {
		m_value = other.m_value;
		call_count = other.call_count;
		other.m_value = nullptr;
		++(call_count->constructor);
		++(call_count->move_constructor);
	}
	~TestCls() {
		++(call_count->destructor);
		if (m_value != nullptr) {
			delete m_value;
		}
		// Cleanup pointer
		m_value = nullptr;
	}
	TestCls& operator=(const TestCls &other) {
		if (this != &other) {
			*m_value = *other.m_value;
		}
		call_count = other.call_count;
		++(call_count->copy);
		return *this;
	}
	TestCls& operator=(TestCls &&other) {
		if (this != &other) {
			m_value = other.m_value;
			other.m_value = nullptr;
		}
		call_count = other.call_count;
		++(call_count->move);
		return *this;
	}

	void operator*=(unsigned value) {
		*m_value *= value;
		++(call_count->multiplication_assignment);
	}

	int value() const {
		return *m_value;
	}
	bool is_null() const {
		return m_value == nullptr;
	}

	/*! call after constructions, used for construction after an array allocation */
	void attach(Counter* counter) {
		call_count = counter;
		call_count->constructor++; 
	}
};