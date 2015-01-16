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
	reset_counters();
	{
		ArrayList<TestCls> list;
	}
	ASSERT_EQ(constructor_call_count, destructor_call_count);
#ifdef TEST_CALL_COUNTS
	ASSERT_EQ(0, constructor_call_count);
	ASSERT_EQ(0, destructor_call_count);
#endif
}

TEST(TestArrayList, constructor_with_size_test001)
{
	reset_counters();
	{
		ArrayList<TestCls> list(3);
		ASSERT_EQ(3, list.size());
	}
	ASSERT_EQ(constructor_call_count, destructor_call_count);
#ifdef TEST_CALL_COUNTS
	ASSERT_EQ(3, constructor_call_count);
	ASSERT_EQ(3, destructor_call_count);
#endif
}

TEST(TestArrayList, push_back_test001)
{
	TestCls v1(1);
	TestCls v2(2);
	//TestCls v3(3);
	reset_counters();
	{
		ArrayList<TestCls> list;
		list.push_back(v1);
		list.push_back(v2);
		list.push_back(TestCls(3));
		ASSERT_EQ(3, list.count());
		ASSERT_EQ(1, list[0].value());
		ASSERT_EQ(2, list[1].value());
		ASSERT_EQ(3, list[2].value());
	}
	ASSERT_EQ(constructor_call_count, destructor_call_count);
#ifdef TEST_CALL_COUNTS
	ASSERT_EQ(6, constructor_call_count);
	ASSERT_EQ(2, copy_call_count);
	ASSERT_EQ(1, move_call_count);
	ASSERT_EQ(6, destructor_call_count);
#endif
}

// Try to add objects more than reserve 
TEST(TestArrayList, push_back_test002)
{
	TestCls v1(1);
	TestCls v2(2);
	TestCls v3(3);
	TestCls v4(4);
	TestCls v5(5);
	TestCls v6(6);
	reset_counters();
	{
		ArrayList<TestCls> list;
		list.push_back(v1);
		list.push_back(v2);
		list.push_back(v3);
		list.push_back(v4);
		list.push_back(v5);
		list.push_back(v6);
		ASSERT_EQ(6, list.count());
		ASSERT_EQ(1, list[0].value());
		ASSERT_EQ(2, list[1].value());
		ASSERT_EQ(3, list[2].value());
		ASSERT_EQ(4, list[3].value());
		ASSERT_EQ(5, list[4].value());
		ASSERT_EQ(6, list[5].value());
	}
	ASSERT_EQ(constructor_call_count, destructor_call_count);
#ifdef TEST_CALL_COUNTS
	ASSERT_EQ(6, copy_call_count);
#endif
}

TEST(TestArrayList, copy_constructor_test001)
{
	TestCls v1(1);
	TestCls v2(2);
	TestCls v3(3);

	ArrayList<TestCls> list;
	list.push_back(v1);
	list.push_back(v2);
	list.push_back(v3);

	reset_counters();
	{
		ArrayList<TestCls> list2(list);
		ASSERT_EQ(3, list2.count());
		ASSERT_EQ(1, list2[0].value());
		ASSERT_EQ(2, list2[1].value());
		ASSERT_EQ(3, list2[2].value());
	}
	ASSERT_EQ(constructor_call_count, destructor_call_count);
#ifdef TEST_CALL_COUNTS
	ASSERT_EQ(5, constructor_call_count);
	ASSERT_EQ(3, copy_call_count);
	ASSERT_EQ(5, destructor_call_count);
#endif
}
 
TEST(TestArrayList, move_constructor_test001)
{
	TestCls v1(1);
	TestCls v2(2);
	TestCls v3(3);

	reset_counters();
	{
		ArrayList<TestCls> list;
		list.push_back(v1);
		list.push_back(v2);
		list.push_back(v3);

		ArrayList<TestCls> list2(std::move(list));
		ASSERT_EQ(3, list2.count());
		ASSERT_EQ(1, list2[0].value());
		ASSERT_EQ(2, list2[1].value());
		ASSERT_EQ(3, list2[2].value());
	}
	ASSERT_EQ(constructor_call_count, destructor_call_count);
#ifdef TEST_CALL_COUNTS
	// 5 because the list will reserve 5 and the std::move is a simple pointer swap
	ASSERT_EQ(5, constructor_call_count);
	ASSERT_EQ(3, copy_call_count); 
	ASSERT_EQ(5, destructor_call_count);
#endif
} 


