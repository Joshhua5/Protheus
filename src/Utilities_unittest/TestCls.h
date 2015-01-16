#pragma once

static int constructor_call_count = 0;
static int copy_constructor_call_count = 0;
static int move_constructor_call_count = 0;
static int destructor_call_count = 0;
static int copy_call_count = 0;
static int move_call_count = 0;
static int multiplication_assignment = 0;

static void reset_counters()
{
	constructor_call_count = 0;
	copy_constructor_call_count = 0;
	move_constructor_call_count = 0;
	destructor_call_count = 0;
	copy_call_count = 0;
	move_call_count = 0; 
	multiplication_assignment = 0;
}

class TestCls {
private:
	int *m_value = nullptr;
public:
	TestCls() {
		m_value = new int(0);
		++constructor_call_count;
	}
	TestCls(int value) {
		m_value = new int(value);
		++constructor_call_count;
	}
	TestCls(const TestCls &other) {
		m_value = new int(*other.m_value);
		++constructor_call_count;
		++copy_constructor_call_count;
	}
	TestCls(TestCls &&other) {
		m_value = other.m_value;
		other.m_value = nullptr;
		++constructor_call_count;
		++move_constructor_call_count;
	}
	~TestCls() {
		if (m_value != nullptr) {
			delete m_value;
		}
		// Cleanup pointer
		m_value = nullptr;
		++destructor_call_count;
	}
	TestCls& operator=(const TestCls &other) {
		if (this != &other) {
			*m_value = *other.m_value;
		}
		++copy_call_count;
		return *this;
	}
	TestCls& operator=(TestCls &&other) {
		if (this != &other) {
			m_value = other.m_value;
			other.m_value = nullptr;
		}
		++move_call_count;
		return *this;
	}

	void operator*=(unsigned value) {
		*m_value *= value;
		multiplication_assignment++;
	}

	int value() const {
		return *m_value;
	}
	bool is_null() const {
		return m_value == nullptr;
	}
};