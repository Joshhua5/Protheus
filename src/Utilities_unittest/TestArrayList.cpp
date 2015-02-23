#include <iostream>
#include <vector>
#include "gtest/gtest.h"
#include "TestCls.h"
#include "ArrayList.h"
  
template <typename T>
using ArrayList = Pro::Util::ArrayList<T>;


TEST(TestArrayList, default_constructor_test001)
{
	Counter call_count;
	{
		ArrayList<TestCls> list;
	}
	ASSERT_EQ(call_count.constructor, call_count.destructor); 
	ASSERT_EQ(0, call_count.constructor);
	ASSERT_EQ(0, call_count.destructor); 
}

TEST(TestArrayList, constructor_with_size_test001)
{
	Counter call_count;
	{
		ArrayList<TestCls> list(3, &call_count);
		ASSERT_EQ(3, list.size());
		ASSERT_EQ(3, list.capacity());
	}
	ASSERT_EQ(call_count.constructor, call_count.destructor); 
	ASSERT_EQ(3, call_count.constructor);
	ASSERT_EQ(3, call_count.destructor); 
}

TEST(TestArrayList, push_back_test001)
{
	Counter call_count;
	TestCls v1(&call_count, 1);
	TestCls v2(&call_count, 2);
	call_count.reset();
	{
		ArrayList<TestCls> list;
		list.PushBack(v1);
		list.PushBack(v2);
		// twP constructor calls here, use emplace for 1
		list.PushBack(TestCls(&call_count, 3));
		ASSERT_EQ(3, list.size());
		ASSERT_EQ(1, list[0].value());
		ASSERT_EQ(2, list[1].value());
		ASSERT_EQ(3, list[2].value());
	}
	ASSERT_EQ(call_count.constructor, call_count.destructor); 
	ASSERT_EQ(4, call_count.constructor);
	ASSERT_EQ(0, call_count.copy);
	ASSERT_EQ(0, call_count.move);
	ASSERT_EQ(4, call_count.destructor); 
}

// Try to add objects more than reserve 
TEST(TestArrayList, push_back_test002)
{
	Counter call_count;
	TestCls v1(&call_count, 1);
	TestCls v2(&call_count, 2);
	TestCls v3(&call_count, 3);
	TestCls v4(&call_count, 4);
	TestCls v5(&call_count, 5);
	TestCls v6(&call_count, 6);
	call_count.reset();
	{
		ArrayList<TestCls> list;
		list.Reserve(6);
		list.PushBack(v1);
		list.PushBack(v2);
		list.PushBack(v3);
		list.PushBack(v4);
		list.PushBack(v5);
		list.PushBack(v6);
		ASSERT_EQ(6, list.size());
		ASSERT_EQ(1, list[0].value());
		ASSERT_EQ(2, list[1].value());
		ASSERT_EQ(3, list[2].value());
		ASSERT_EQ(4, list[3].value());
		ASSERT_EQ(5, list[4].value());
		ASSERT_EQ(6, list[5].value());
	}
	ASSERT_EQ(call_count.constructor, call_count.destructor); 
	ASSERT_EQ(6, call_count.constructor); 
}

TEST(TestArrayList, copy_constructor_test001)
{
	Counter call_count;
	TestCls v1(&call_count, 1);
	TestCls v2(&call_count, 2);
	TestCls v3(&call_count, 3);

	ArrayList<TestCls> list;
	list.PushBack(v1);
	list.PushBack(v2);
	list.PushBack(v3);

	call_count.reset();
	{
		ArrayList<TestCls> list2(list);
		ASSERT_EQ(3, list2.size());
		ASSERT_EQ(1, list2[0].value());
		ASSERT_EQ(2, list2[1].value());
		ASSERT_EQ(3, list2[2].value());
	}
	ASSERT_EQ(call_count.constructor, call_count.destructor); 
	ASSERT_EQ(3, call_count.constructor); 
}

TEST(TestArrayList, move_constructor_test001)
{
	Counter call_count;
	TestCls v1(&call_count, 1);
	TestCls v2(&call_count, 2);
	TestCls v3(&call_count, 3);

	call_count.reset();
	{
		ArrayList<TestCls> list;
		list.PushBack(v1);
		list.PushBack(v2);
		list.PushBack(v3);

		ArrayList<TestCls> list2(std::move(list));
		ASSERT_EQ(3, list2.size());
		ASSERT_EQ(1, list2[0].value());
		ASSERT_EQ(2, list2[1].value());
		ASSERT_EQ(3, list2[2].value());
	}
	ASSERT_EQ(call_count.constructor, call_count.destructor); 
	// 5 because the list will reserve 5 and the std::move is a simple pointer swap
	ASSERT_EQ(3, call_count.constructor); 
}


TEST(TestArrayList, erase_test001) {
	ArrayList<unsigned> list(10);
	list.Erase({ 2, 4, 5 });
	ASSERT_EQ(7, list.size());
}

TEST(TestArrayList, erase_test002) {
	ArrayList<unsigned> list(10);
	list.Erase({ 5, 4, 2 });
	ASSERT_EQ(7, list.size());
}

TEST(TestArrayList, erase_test003) {
	ArrayList<unsigned> list(10);
	list.Erase({ 2, 20 });
	ASSERT_EQ(9, list.size());
}

TEST(TestArrayList, erase_test004) {
	ArrayList<unsigned> list(10);
	list.Erase({ 200, 2 });
	ASSERT_EQ(9, list.size());
}

TEST(TestArrayList, resize_test001){
	ArrayList<unsigned> list(10);
	list.Resize(20);
	list.Resize(30);
	list.Resize(10);
	ASSERT_EQ(10, list.size());
	ASSERT_EQ(10, list.capacity());
}

TEST(TestArrayList, reserve_test001){
	ArrayList<unsigned> list;
	list.Reserve(20);
	list.Reserve(30);
	ASSERT_EQ(30, list.capacity());
	ASSERT_EQ(0, list.size()); 
}