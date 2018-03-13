#include "reversion_function.h"
#include "pg_list_utils.h"
#include "gtest/gtest.h"
#include <memory>


class ReversionFunctionTest : public ::testing::Test
{
public:
	ReversionFunctionTest() = default;
};

TEST_F(ReversionFunctionTest, empty)
{
	ListWrapper list;
	EXPECT_STREQ(L"", reversion_function(&list));
}

TEST_F(ReversionFunctionTest, example)
{
	ListWrapper list;
	list_append(list, { L"MyApp", L"MyModule", L"MyTable" });
	EXPECT_STREQ(L"MyTable.MyModule.MyApp", reversion_function(&list));
}

TEST_F(ReversionFunctionTest, long_list)
{
	ListWrapper list;
	std::wstring expected;
	for (auto i = 0; i < 100; ++i) {
		list_append(list, { L"MyApp", L"MyModule", L"MyTable" });
		expected += L"MyTable.MyModule.MyApp.";
	}
	expected.pop_back();
	EXPECT_STREQ(expected.c_str(), reversion_function(&list));
}

TEST_F(ReversionFunctionTest, very_long_list)
{
	ListWrapper list;
	std::wstring expected;
	for (auto i = 0; i < 1000000; ++i) {
		list_append(list, { L"MyApp", L"MyModule", L"MyTable" });
		expected += L"MyTable.MyModule.MyApp.";
	}
	expected.pop_back();
	EXPECT_STREQ(expected.c_str(), reversion_function(&list));
}

