#include "reversion_function.h"
#include "pg_list_utils.h"
#include "gtest/gtest.h"


TEST(pg_list, list_append)
{
	ListWrapper list;
	list_append(list, { L"aaa" });
	EXPECT_EQ(list.head, list.tail);
	EXPECT_EQ(1, list.length);
	EXPECT_STREQ(L"aaa", list_item_name(list));
	list_clear(list);
	list_append(list, { L"MyApp", L"MyModule", L"MyTable" });
	EXPECT_STREQ(L"MyApp", list_item_name(list, 0));
	EXPECT_STREQ(L"MyModule", list_item_name(list, 1));
	EXPECT_STREQ(L"MyTable", list_item_name(list, 2));
}

