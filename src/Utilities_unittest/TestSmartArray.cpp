#pragma once

#include "gtest\gtest.h"
#include "TestCls.h"
#include <smart_array.h>


using namespace Pro::Util;
 

TEST(Smart_Array, Constructor_Deconstructor_Test) {
	Counter call_count;
	smart_array<TestCls> x = nullptr;

	x = new TestCls[20];
	for (unsigned i = 0; i < 20; ++i)
		x[i].attach(&call_count);

	TestCls* ptr = x.get();
	x.Dereference();

	ASSERT_EQ(x.get(), nullptr);
	ASSERT_EQ(call_count.destructor, 20);
	call_count.reset();

	x = new TestCls[20];
	for (unsigned i = 0; i < 20; ++i)
		x[i].attach(&call_count);
	smart_array<TestCls> y = x;

	ASSERT_EQ(y.references(), 2);
	ASSERT_EQ(call_count.destructor, 0);
	x = nullptr;
	ASSERT_EQ(call_count.destructor, 0);
	y = nullptr;
	ASSERT_EQ(call_count.destructor, 20);
}


void funcTest(smart_array<TestCls> obj) {
	obj.get();
}


TEST(Smart_Array, Copy_Test) {
	Counter call_count; 
	TestCls* original = new TestCls[20];
	for (unsigned i = 0; i < 20; ++i)
		original[i].attach(&call_count);
	smart_array<TestCls> x = original;
	smart_array<TestCls> y = x;
	smart_array<TestCls> z = y;
	funcTest(z);

	ASSERT_EQ(x.references(), 3);
	x.Dereference();
	y.Dereference();
	ASSERT_EQ(z.get(), original);
	ASSERT_EQ(z.get()->value(), 0);
	ASSERT_EQ(z.references(), 1);
	ASSERT_EQ(call_count.destructor, 0);
	z.Dereference();
	ASSERT_EQ(call_count.destructor, 20);
}


TEST(Smart_Array, Move_Test) {
	Counter call_count; 
	TestCls* original = new TestCls[20];
	for (unsigned x = 0; x < 20; ++x)
		original[x].attach(&call_count);
	smart_array<TestCls> x = original;
	smart_array<TestCls> y = x;
	smart_array<TestCls> z = std::move(y);

	ASSERT_EQ(z.references(), 2);
	ASSERT_EQ(y.get(), nullptr);
	ASSERT_EQ(z.get(), original);
	ASSERT_EQ(z.get()->value(), 0);
	ASSERT_EQ(call_count.destructor, 0);
	z.Dereference();
	x.Dereference();
	ASSERT_EQ(call_count.destructor, 20);
}