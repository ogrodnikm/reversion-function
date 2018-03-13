#pragma once

#include "pg_list.h"
#include <iterator>


static inline const wchar_t* get_string(const ListCell& cell)
{ 
	return reinterpret_cast<const Ident*>(cell.data.ptr_value)->name; 
}

template<typename CellType>
class pg_list_iterator
{
public:
	using self_type = pg_list_iterator;
	using reference = CellType &;
	using pointer = CellType *;
	using value_type = CellType;
	using difference_type = std::ptrdiff_t;
	using iterator_category = std::forward_iterator_tag;

	pg_list_iterator(pointer ptr = nullptr) : _ptr(ptr) {}
	pg_list_iterator(const self_type& it) : _ptr(it._ptr) {}
	pg_list_iterator& operator=(const self_type& it) { _ptr = it._ptr; }

	self_type& operator++() {
		_ptr = _ptr->next;
		return *this;
	}
	self_type operator++(int) {
		self_type result(*this);
		_ptr = _ptr->next;
		return result;
	}
	self_type operator+(difference_type num) const {
		self_type new_it(*this);
		new_it += num;
		return new_it;
	}
	self_type& operator+=(difference_type num) {
		// warning O(n) complexity
		for (auto i = 0u; i < num; ++i)
			_ptr = _ptr->next;
		return *this;
	}
	reference operator*() { return *_ptr; }
	pointer operator->() { return _ptr; }
	bool operator==(const self_type& rhs) {
		return _ptr == rhs._ptr;
	}
	bool operator!=(const self_type& rhs) {
		return _ptr != rhs._ptr;
	}
private:
	pointer _ptr;
};

class PgListAdapterBase
{
	List* _list;

public:
	using value_type = const wchar_t*;
	using iterator = pg_list_iterator<ListCell>;
	using const_iterator = pg_list_iterator<const ListCell>;

public:
	PgListAdapterBase(List* list) : _list(list) {}
	iterator begin() { return _list->head; }
	const_iterator begin() const { return _list->head; }
	iterator end() { return nullptr; }
	const_iterator end() const { return nullptr; }
	std::size_t size() const { return _list->length; }
	void reverse()
	{
		ListCell* old_tail = _list->tail;
		while (_list->head != old_tail)
		{
			ListCell* moved = _list->head;
			_list->head = moved->next;
			moved->next = old_tail->next;
			old_tail->next = moved;
		}
	}
};

struct PgListAdapter : PgListAdapterBase
{
	PgListAdapter(List* list) : PgListAdapterBase(list) {}
};

struct PgListConstAdapter : PgListAdapterBase
{
	using iterator = PgListAdapterBase::const_iterator;
	PgListConstAdapter(const List* list) : PgListAdapterBase(const_cast<List*>(list)) {}
};
