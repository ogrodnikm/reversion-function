#include "reversion_function_impl.h"
#include "gtest/gtest.h"
#include <memory>
#include <list>
#include <forward_list>


class GeneralizedReversionFunctionTest : public ::testing::Test
{
public:
	GeneralizedReversionFunctionTest() = default;
};

TEST_F(GeneralizedReversionFunctionTest, v_p_wchar_t)
{
	std::vector<const wchar_t*> v_char = { L"MyApp", L"MyModule", L"MyTable" };
	EXPECT_STREQ(L"MyTable.MyModule.MyApp", reversion_function(v_char));
}

TEST_F(GeneralizedReversionFunctionTest, v_p_char)
{
	std::vector<const char*> v_char = { "MyApp", "MyModule", "MyTable" };
	EXPECT_STREQ("MyTable.MyModule.MyApp", reversion_function(v_char));
}

TEST_F(GeneralizedReversionFunctionTest, v_string)
{
	std::vector<std::string> v_string = { "MyApp", "MyModule", "MyTable" };
	EXPECT_STREQ("MyTable.MyModule.MyApp", reversion_function(v_string));
}

TEST_F(GeneralizedReversionFunctionTest, v_string_view)
{
	std::vector<std::string_view> v_string = { "MyApp", "MyModule", "MyTable" };
	EXPECT_STREQ("MyTable.MyModule.MyApp", reversion_function(v_string));
}

TEST_F(GeneralizedReversionFunctionTest, l_string_view)
{
	std::list<std::string_view> v_string = { "MyApp", "MyModule", "MyTable" };
	EXPECT_STREQ("MyTable.MyModule.MyApp", reversion_function(v_string));
}

TEST_F(GeneralizedReversionFunctionTest, fl_string_view)
{
	std::forward_list<std::string_view> v_string = { "MyApp", "MyModule", "MyTable" };
	EXPECT_STREQ("MyTable.MyModule.MyApp", reversion_function(v_string));
}
