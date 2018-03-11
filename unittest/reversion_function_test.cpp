#include "reversion_function.h"
#include "pg_list_utils.h"
#include "gtest/gtest.h"
#include <memory>


class ReversionFunctionTest : public ::testing::Test
{
	std::unique_ptr<ListWrapper> _list;

public:
	ReversionFunctionTest() = default;
	List* buildExampleList()
	{
		_list = std::make_unique<ListWrapper>();
		list_append(*_list, { L"MyApp", L"MyModule", L"MyTable" });
		return _list.get();
	}
};


TEST_F(ReversionFunctionTest, reversion_function)
{
	EXPECT_STREQ(L"MyTable.MyModule.MyApp", reversion_function(buildExampleList()));
}