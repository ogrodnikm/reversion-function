#include "gtest/gtest.h"
#include "adapters/pg_list_adapter.h"
#include "pg_list_utils.h"
#include "reversion_function_impl.h"
#include <memory>


class PgListAdapterTest : public ::testing::Test
{
	ListWrapper _example_list;

public:
	PgListAdapterTest()
	{
		list_append(_example_list, { L"MyApp", L"MyModule", L"MyTable" });
	}
	auto build_example_list_adapter()
	{
		return std::make_unique<PgListAdapter>(&_example_list);
	}
};

TEST_F(PgListAdapterTest, get_string)
{
	auto p_adapter = build_example_list_adapter();
	EXPECT_STREQ(L"MyApp", get_string(*p_adapter->begin()));
}

TEST_F(PgListAdapterTest, get_char_t)
{
	static_assert(std::is_same_v<get_char_t<PgListAdapter>, wchar_t>, 
		"adapter value type is not same as pg_list character type");
}

TEST_F(PgListAdapterTest, loop)
{
	auto p_adapter = build_example_list_adapter();
	auto count = 0;
	for (auto& element : *p_adapter)
		++count;
	EXPECT_EQ(p_adapter->size(), count);
}
