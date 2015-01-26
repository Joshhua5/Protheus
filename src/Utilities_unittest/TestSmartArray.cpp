#include "gtest\gtest.h"
#include <smart_array.h>

#pragma once

using namespace Pro::Util;

static unsigned destructor_call_count = 0;

class testObj {
public:
	unsigned value;
	testObj() { value = 2; }
	testObj(unsigned u) { value = u; }
	~testObj() { destructor_call_count++; }
};

static void reset() {
	destructor_call_count = 0;
}

TEST(Smart_Array, Constructor_Deconstructor_Test) {
	smart_array<testObj> x = nullptr;

	x = new testObj[20];

	testObj* ptr = x.get();
	x.dereference();

	ASSERT_EQ(x.get(), nullptr);
	ASSERT_EQ(destructor_call_count, 1);
	reset();

	x = new testObj[20];
	smart_array<testObj> y = x;

	ASSERT_EQ(y.references(), 2);
	ASSERT_EQ(destructor_call_count, 0);
	x = nullptr;
	ASSERT_EQ(destructor_call_count, 0);
}


void funcTest(smart_array<testObj> obj) {
	obj.get();
}
 

TEST(Smart_Array, Copy_Test) {
	reset();
	testObj* original = new testObj[20];
	smart_array<testObj> x = original;
	smart_array<testObj> y = x;
	smart_array<testObj> z = y;
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


TEST(Smart_Array, Move_Test) {
	reset();
	testObj* original = new testObj[20];
	smart_array<testObj> x = original;
	smart_array<testObj> y = x;
	smart_array<testObj> z = std::move(y);

	ASSERT_EQ(z.references(), 2);
	ASSERT_EQ(y.get(), nullptr);
	ASSERT_EQ(z.get(), original);
	ASSERT_EQ(z.get()->value, 2);
	ASSERT_EQ(destructor_call_count, 0);
	z.dereference();
	x.dereference();
	ASSERT_EQ(destructor_call_count, 1);
}