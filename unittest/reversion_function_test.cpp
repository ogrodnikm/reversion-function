#include "reversion_function.h"
#include "pg_list_utils.h"
#include "gtest/gtest.h"
#include <memory>


class ReversionFunctionTest : public ::testing::Test
{
	std::unique_ptr<ListWrapper> _list;

public:
	ReversionFunctionTest() = default;
	List& buildExampleList()
	{
		_list = std::make_unique<ListWrapper>();
		list_append(*_list, { L"MyApp", L"MyModule", L"MyTable" });
		return *_list;
	}
};


TEST_F(ReversionFunctionTest, reversion_function)
{
	//List(ListCell(Ident(name:"MyApp"))->ListCell(Ident(name:"MyModule"))->ListCell(Ident(name:"MyTable"))->null)a;
	//EXPECT_STREQ(L"aaa", reversion_function());
}