#include <reversion_function.h>
#include <memory>
#include <string_view>
#include <gtest/gtest.h>

// TODO: move to common, make independent of Ident
void list_clear(List& list)
{
	ListCell* current = list.head;
	while (current)
	{
		ListCell* next = current->next;
		delete [] reinterpret_cast<Ident*>(current->data.ptr_value)->name;
		delete current->data.ptr_value;
		delete current;
		current = next;
	}
	list.length = 0;
	list.head = nullptr;
	list.tail = nullptr;
}

//template<typename Iterable>
// TODO: template, wstring_view
void list_append(List& list, const std::initializer_list<const wchar_t*>& items)
{
	// add items starting from tail
	for (auto& item : items)
	{
		ListCell* cell = new ListCell;
		if (!list.head) 
			list.head = cell; // this is the first element on the list
		if (list.tail)
			list.tail->next = cell;
		list.tail = cell; 
		++list.length;

		Ident* ident = new Ident;
		cell->data.ptr_value = ident;
		cell->next = nullptr;
		auto size = wcslen(item) + 1;
		ident->name = new wchar_t[size];
		wcscpy(const_cast<wchar_t*>(ident->name), item);
		//ident->name = new wchar_t[item.size()];
		//item.copy(ident->name, item.size());
	}
}

const ListCell* list_item(const List& list, unsigned num = 0)
{
	const ListCell* cell = list.head;
	for (auto i = 0u; i < num; ++i)
		cell = cell->next;
	return cell;
}

const wchar_t* list_item_name(const List& list, unsigned num = 0)
{
	return reinterpret_cast<Ident*>(list_item(list, num)->data.ptr_value)->name;
}

struct ListWrapper : public List
{
	ListWrapper() : List({}) {}
	~ListWrapper() 
	{
		list_clear(*this);
	}
};


class ReversionFunctionTest : public ::testing::Test
{
	std::unique_ptr<ListWrapper> _list;

public:
	ReversionFunctionTest() = default;
	List& buildExampleList()
	{
		_list = std::make_unique<ListWrapper>();
		_list->head = new ListCell;

	}
};

TEST_F(ReversionFunctionTest, list_append)
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

TEST_F(ReversionFunctionTest, reversion_function)
{
	//List(ListCell(Ident(name:"MyApp"))->ListCell(Ident(name:"MyModule"))->ListCell(Ident(name:"MyTable"))->null)a;
	//EXPECT_STREQ(L"aaa", reversion_function());
}