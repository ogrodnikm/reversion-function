#pragma once

#include "pg_list.h"
#include <string_view>

// This option requires usage of compiletime constant string
#define COPY_STRING
#undef COPY_STRING

static inline
const auto* get_name(const ListCell* const listEl)
{
	return reinterpret_cast<const Ident*>(listEl->data.ptr_value)->name;
}

static inline
void list_clear(List& list)
{
	ListCell* current = list.head;
	while (current)
	{
		ListCell* next = current->next;
#ifdef COPY_STRING
		delete[] reinterpret_cast<Ident*>(current->data.ptr_value)->name;
#endif
		delete current->data.ptr_value;
		delete current;
		current = next;
	}
	list.length = 0;
	list.head = nullptr;
	list.tail = nullptr;
}

static inline
void list_append(List& list, const std::initializer_list<std::wstring_view>& items)
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
#ifdef COPY_STRING
		ident->name = new wchar_t[item.size() + 1];
		item.copy(const_cast<wchar_t*>(ident->name), item.size());
		const_cast<wchar_t*>(ident->name)[item.size()] = L'\0';
#else
		ident->name = item.data();
#endif
	}
}

static inline
const ListCell* list_item(const List& list, unsigned num = 0)
{
	const ListCell* cell = list.head;
	for (auto i = 0u; i < num; ++i)
		cell = cell->next;
	return cell;
}

static inline
const wchar_t* list_item_name(const List& list, unsigned num = 0)
{
	return get_name(list_item(list, num));
}

static inline
void list_reverse(List& list)
{
	ListCell* old_tail = list.tail;
	while (list.head != old_tail)
	{
		ListCell* moved = list.head;
		list.head = moved->next;
		moved->next = old_tail->next;
		old_tail->next = moved;
	}
}

struct ListWrapper : public List
{
	ListWrapper() : List({}) {}
	~ListWrapper()
	{
		list_clear(*this);
	}
};
