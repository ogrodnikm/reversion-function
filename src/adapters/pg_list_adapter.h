#pragma once

#include "pg_list.h"
#include <iterator>

/**
* Iterator for pg_list. Provides capability for pg_list to be used with C++ algorithms.
* templated in order to easily provide const and non-const version.
* Could be generalized to provide iterator capability for all C-style forward lists.
*/
template<typename CellType>
class list_iterator
{
public:
	using self_type = list_iterator;
	using reference = CellType & ;
	using pointer = CellType * ;
	using value_type = CellType;
	using difference_type = std::ptrdiff_t;
	using iterator_category = std::forward_iterator_tag;

	list_iterator(pointer ptr = nullptr) : _ptr(ptr) {}
	list_iterator(const self_type& it) : _ptr(it._ptr) {}
	list_iterator& operator=(const self_type& it) { _ptr = it._ptr; }

	reference operator*() { return *_ptr; }
	pointer operator->() { return _ptr; }
	bool operator==(const self_type& rhs) { return _ptr == rhs._ptr; }
	bool operator!=(const self_type& rhs) { return _ptr != rhs._ptr; }

	self_type& operator++()
	{
		_ptr = _ptr->next;
		return *this;
	}

	/* Those functions are not required in adapter implementation

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
	*/
private:
	pointer _ptr;
};

/**
* Base class for PgListAdapter. Provides C++ light container wrapper over C-style pg list to 
* facilitatie it for reversion_function algorithm.
*/
class PgListAdapterBase
{
public:
	using value_type = const wchar_t*;
	using iterator = list_iterator<ListCell>;
	using const_iterator = list_iterator<const ListCell>;

	PgListAdapterBase(List* list) : _list(list) {}

	std::size_t size() const { return _list->length; }
	const_iterator begin() const { return _list->head; }
	const_iterator end() const { return nullptr; }

protected:
	List * _list;
};

/**
* PgListAdapter for const List.
*/
struct PgListConstAdapter : PgListAdapterBase
{
	using iterator = const_iterator;

	PgListConstAdapter(const List* list) : PgListAdapterBase(const_cast<List*>(list)) {}
};

/**
* PgListAdapter for non-const List. Adds reverse() function to possibly achieve better performance.
*/
struct PgListAdapter : PgListAdapterBase
{
	PgListAdapter(List* list) : PgListAdapterBase(list) {}

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


/**
* Provides way to retrieve string from value returned by pg_list_iterator.
*/
static inline const wchar_t* get_string(const ListCell& cell)
{
	return reinterpret_cast<const Ident*>(cell.data.ptr_value)->name;
}
