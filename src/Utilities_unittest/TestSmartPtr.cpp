#pragma once

#include "gtest\gtest.h"
#include <smart_ptr.h>
using namespace Pro::Util;

static unsigned destructor_call_count = 0;

class testObj {
public:
	unsigned value;
	testObj(unsigned u) { value = u; }
	~testObj() { destructor_call_count++; }
};

void reset() {
	destructor_call_count = 0;
}

TEST(Smart_Ptr, Constructor_Deconstructor_Test) {
	smart_ptr<testObj> x = nullptr;

	x = new testObj(2);

	testObj* ptr = x.get();
	x.dereference();

	ASSERT_EQ(x.get(), nullptr);
	ASSERT_EQ(destructor_call_count, 1);
	reset();

	x = new testObj(2);
	smart_ptr<testObj> y = x;

	ASSERT_EQ(y.references(), 2);
	ASSERT_EQ(destructor_call_count, 0); 
	x = nullptr; 
	ASSERT_EQ(destructor_call_count, 0);
}


void funcTest(smart_ptr<testObj> obj) {
	obj.get();
}

void funcTestm(smart_ptr<testObj>&& obj) {
	obj.get();
}

TEST(Smart_Ptr, Copy_Test) {
	reset();
	testObj* original = new testObj(2);
	smart_ptr<testObj> x = original;
	smart_ptr<testObj> y = x;
	smart_ptr<testObj> z = y;
	funcTest(z);

	ASSERT_EQ(x.references(), 3);
	x.dereference();
	y.dereference();
	ASSERT_EQ(z.get(), original);
	ASSERT_EQ(z.get()->value, 2);
	ASSERT_EQ(z.references(), 1);
	ASSERT_EQ(destructor_call_count, 0);
	z.dereference();
	ASSERT_EQ(destructor_call_count, 1);
}


TEST(Smart_Ptr, Move_Test) {
	reset();
	testObj* original = new testObj(2);
	smart_ptr<testObj> x = original;
	smart_ptr<testObj> y = x;
	smart_ptr<testObj> z = std::move(y);

	ASSERT_EQ(z.references(), 2);
	ASSERT_EQ(y.get(), nullptr); 
	ASSERT_EQ(z.get(), original);
	ASSERT_EQ(z.get()->value, 2); 
	ASSERT_EQ(destructor_call_count, 0);
	z.dereference();
	x.dereference();
	ASSERT_EQ(destructor_call_count, 1);
}