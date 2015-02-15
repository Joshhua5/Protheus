#pragma once

#include "gtest\gtest.h"
#include "TestCls.h"
#include <smart_ptr.h>

using namespace Pro::Util;
  

TEST(Smart_Ptr, Constructor_Deconstructor_Test) {
	Counter call_count;
	smart_ptr<TestCls> x = nullptr;

	x = new TestCls(&call_count, 2);

	TestCls* ptr = x.get();
	x.Dereference();

	ASSERT_EQ(x.get(), nullptr);
	ASSERT_EQ(call_count.destructor, 1);
	call_count.reset();

	x = new TestCls(&call_count, 2);
	smart_ptr<TestCls> y = x;

	ASSERT_EQ(y.references(), 2);
	ASSERT_EQ(call_count.destructor, 0);
	x = nullptr; 
	y = nullptr;
	ASSERT_EQ(call_count.destructor, 1);
}


void funcTest(smart_ptr<TestCls> obj) {
	obj.get();
}

void funcTestm(smart_ptr<TestCls>&& obj) {
	obj.get();
}

TEST(Smart_Ptr, Copy_Test) {
	Counter call_count; 
	TestCls* original = new TestCls(&call_count, 2);
	smart_ptr<TestCls> x = original;
	smart_ptr<TestCls> y = x;
	smart_ptr<TestCls> z = y;
	funcTest(z);

	ASSERT_EQ(x.references(), 3);
	x.Dereference();
	y.Dereference();
	ASSERT_EQ(z.get(), original);
	ASSERT_EQ(z.get()->value(), 2);
	ASSERT_EQ(z.references(), 1);
	ASSERT_EQ(call_count.destructor, 0);
	z.Dereference();
	ASSERT_EQ(call_count.destructor, 1);
}


TEST(Smart_Ptr, Move_Test) {
	Counter call_count; 
	TestCls* original = new TestCls(&call_count, 2);
	smart_ptr<TestCls> x = original;
	smart_ptr<TestCls> y = x;
	smart_ptr<TestCls> z = std::move(y);

	ASSERT_EQ(z.references(), 2);
	ASSERT_EQ(y.get(), nullptr); 
	ASSERT_EQ(z.get(), original);
	ASSERT_EQ(z.get()->value(), 2);
	ASSERT_EQ(call_count.destructor, 0);
	z.Dereference();
	x.Dereference();
	ASSERT_EQ(call_count.destructor, 1);
}