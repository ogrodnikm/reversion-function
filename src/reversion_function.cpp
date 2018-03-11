#include "reversion_function.h"
#include "pg_list_utils.h"
#include <vector>
#include <numeric>
#include <algorithm>
#include <string>
#include <functional>

// TODO inependent of char type, container
auto pg_list_to_vstring_view(const List* const list)
{
	std::vector<std::wstring_view> items;
	items.reserve(list->length);
	for (const auto* listEl = list->head; listEl; listEl = listEl->next)
		items.emplace_back(get_name(listEl));
	return items;
}

auto get_target_string_length(const std::vector<std::wstring_view>& items)
{
	std::size_t length = 0;
	return std::accumulate(std::cbegin(items), std::cend(items), length, [](auto sum, const auto& s) {return sum + s.size() + 1; });
}


#define SIMPLE
#undef SIMPLE
#define NO_STD_STRING
#undef NO_STD_STRING
#define RECURSIVE
//#undef RECURSIVE

#ifdef SIMPLE
// This is first implementation that came up in my mind, inefficient, but clear
wchar_t* reversion_function(const List* const list)
{
	std::vector<std::wstring_view> items = pg_list_to_vstring_view(list);
	auto reversed = std::accumulate(std::rbegin(items), std::rend(items), std::wstring(), [](auto& s, const auto& sw) {
		s += sw;
		s += L'.';
		return s;
	});
	wchar_t* output = new wchar_t[reversed.size()];
	wcscpy(output, reversed.c_str());
	output[reversed.size()-1] = L'\0';
	return output;
}
#endif

#ifdef NO_STD_STRING
// Similar to simple implementation, but performance should be much better, since there are not std::string allocations
wchar_t* reversion_function(const List* const list)
{
	std::vector<std::wstring_view> items = pg_list_to_vstring_view(list);
	auto length = get_target_string_length(items);
	wchar_t* output = new wchar_t[length];
	auto* current = output;
	std::for_each(std::rbegin(items), std::rend(items), [&current](const auto& sw) {
		for (auto& ch : sw) {
			*current++ = ch;
		}
		*current++ = L'.';
	});
	*--current = L'\0';
	return output;
}
#endif

#ifdef RECURSIVE
// Out of curiosity, tried recursive version instead of string_view. 
// Unless the input list is really big, this should not blow the stack quickly.
wchar_t* reversion_function(const List* const list)
{
	auto length = 0;
	for (const auto* listEl = list->head; listEl; listEl = listEl->next)
		length += wcslen(get_name(listEl)) + 1;
	
	wchar_t* output = new wchar_t[length];
	auto* current = output;
	std::function<void(const ListCell* cell)> fun;
	fun = [&fun, &current](const ListCell* cell) {
		if (!cell)
			return;
		fun(cell->next);
		const auto* ch = get_name(cell);
		while (*ch){
			*current++ = *ch++;
		}
		*current++ = L'.';
	};
	fun(list->head);
	*--current = L'\0';
	return output;
}
#endif





