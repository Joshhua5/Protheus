#include <iostream>
#include <vector>
#include "gtest/gtest.h"
#include "TestCls.h"
#include "ArrayList.h"

#define TEST_CALL_COUNTS

template <typename T>
using ArrayList = Pro::Util::ArrayList<T>;


TEST(TestArrayList, default_constructor_test001)
{
	Counter call_count;
	{
		ArrayList<TestCls> list;
	}
	ASSERT_EQ(call_count.constructor, call_count.destructor);
#ifdef TEST_CALL_COUNTS
	ASSERT_EQ(0, call_count.constructor);
	ASSERT_EQ(0, call_count.destructor);
#endif
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
#ifdef TEST_CALL_COUNTS
	ASSERT_EQ(3, call_count.constructor);
	ASSERT_EQ(3, call_count.destructor);
#endif
}

TEST(TestArrayList, push_back_test001)
{
	Counter call_count;
	TestCls v1(&call_count, 1);
	TestCls v2(&call_count, 2); 
	call_count.reset();
	{
		ArrayList<TestCls> list;
		list.push_back(v1);
		list.push_back(v2);
		// two constructor calls here, use emplace for 1
		list.push_back(TestCls(&call_count, 3));
		ASSERT_EQ(3, list.size());
		ASSERT_EQ(1, list[0].value());
		ASSERT_EQ(2, list[1].value());
		ASSERT_EQ(3, list[2].value());
	}
	ASSERT_EQ(call_count.constructor, call_count.destructor);
#ifdef TEST_CALL_COUNTS
	ASSERT_EQ(4, call_count.constructor);
	ASSERT_EQ(0, call_count.copy);
	ASSERT_EQ(0, call_count.move);
	ASSERT_EQ(4, call_count.destructor);
#endif
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
		list.reserve(6);
		list.push_back(v1);
		list.push_back(v2);
		list.push_back(v3);
		list.push_back(v4);
		list.push_back(v5);
		list.push_back(v6);
		ASSERT_EQ(6, list.size());
		ASSERT_EQ(1, list[0].value());
		ASSERT_EQ(2, list[1].value());
		ASSERT_EQ(3, list[2].value());
		ASSERT_EQ(4, list[3].value());
		ASSERT_EQ(5, list[4].value());
		ASSERT_EQ(6, list[5].value());
	}
	ASSERT_EQ(call_count.constructor, call_count.destructor);
#ifdef TEST_CALL_COUNTS
	ASSERT_EQ(6, call_count.constructor);
#endif
}

TEST(TestArrayList, copy_constructor_test001)
{
	Counter call_count;
	TestCls v1(&call_count, 1);
	TestCls v2(&call_count, 2);
	TestCls v3(&call_count, 3);

	ArrayList<TestCls> list;
	list.push_back(v1);
	list.push_back(v2);
	list.push_back(v3);

	call_count.reset();
	{
		ArrayList<TestCls> list2(list);
		ASSERT_EQ(3, list2.size());
		ASSERT_EQ(1, list2[0].value());
		ASSERT_EQ(2, list2[1].value());
		ASSERT_EQ(3, list2[2].value());
	}
	ASSERT_EQ(call_count.constructor, call_count.destructor);
#ifdef TEST_CALL_COUNTS
	ASSERT_EQ(3, call_count.constructor);  
#endif
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
		list.push_back(v1);
		list.push_back(v2);
		list.push_back(v3);

		ArrayList<TestCls> list2(std::move(list));
		ASSERT_EQ(3, list2.size());
		ASSERT_EQ(1, list2[0].value());
		ASSERT_EQ(2, list2[1].value());
		ASSERT_EQ(3, list2[2].value());
	}
	ASSERT_EQ(call_count.constructor, call_count.destructor);
#ifdef TEST_CALL_COUNTS
	// 5 because the list will reserve 5 and the std::move is a simple pointer swap
	ASSERT_EQ(3, call_count.constructor); 
#endif
}


