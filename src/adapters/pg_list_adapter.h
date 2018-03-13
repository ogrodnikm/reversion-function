#pragma once

#include "pg_list.h"
#include <iterator>

class PgListAdapter
{
	const List* _list;

public:
	using value_type = wchar_t;
	using string_type = const value_type*;

public:
	PgListAdapter(const List* list) : _list(list) {}

	// only const version can and needs to be provided
	// reverse_iterator is better choice performance wise, but it's impossible to create from ListCell
	class iterator
	{
	public:
		using self_type = iterator;
		using reference = ListCell & ;
		using pointer = ListCell * ;
		using value_type = ListCell;
		using difference_type = std::ptrdiff_t;
		using iterator_category = std::forward_iterator_tag;

		iterator(pointer ptr = nullptr) : _ptr(ptr) {}
		iterator(const self_type& it) : _ptr(it._ptr) {}
		iterator& operator=(const self_type& it) { _ptr = it._ptr; }

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
		const reference operator*() { return *_ptr; }
		const pointer operator->() { return _ptr; }
		bool operator==(const self_type& rhs) { 
			return _ptr == rhs._ptr; 
		}
		bool operator!=(const self_type& rhs) { 
			return _ptr != rhs._ptr; 
		}
	private:
		pointer _ptr;
	};

	// There is no need go get_string to be there - string extraction could have been resolved on iterator level, 
	// but this feels like a cleaner solution
	static string_type get_string(const ListCell& cell) { return reinterpret_cast<const Ident*>(cell.data.ptr_value)->name; }

	iterator begin() const { return _list->head; }
	iterator end() const { return nullptr; }
	std::size_t size() const { return _list->length; }
};
