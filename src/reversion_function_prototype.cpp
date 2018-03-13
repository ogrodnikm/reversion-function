#include "reversion_function.h"
#include "pg_list_utils.h"
#include <vector>
#include <numeric>
#include <algorithm>
#include <string>


#define SIMPLE
#undef SIMPLE
#define NO_STD_STRING
//#undef NO_STD_STRING
#define NO_STD_STRING_NO_STRING_VIEW
#undef NO_STD_STRING_NO_STRING_VIEW
#define RECURSIVE
#undef RECURSIVE
#define RECURSIVE_LAMBDA
#undef RECURSIVE_LAMBDA

#if defined(SIMPLE) || defined(NO_STD_STRING)
// TODO inependent of char type, container
auto pg_list_to_vstring_view(const List* const list)
{
	std::vector<std::wstring_view> items;
	items.reserve(list->length);
	for (const auto* listEl = list->head; listEl; listEl = listEl->next)
		items.emplace_back(get_name(listEl));
	return items;
}
#endif

#ifdef SIMPLE
// This is first implementation that came up in my mind, very inefficient, but clear
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

auto get_target_string_length(const std::vector<std::wstring_view>& items)
{
	std::size_t length = 0;
	return std::accumulate(std::cbegin(items), std::cend(items), length, [](auto sum, const auto& s) {return sum + s.size() + 1; });
}

// Similar to simple implementation, but performance should be much better, since there are not std::string allocations
wchar_t* reversion_function(const List* const list)
{
	std::vector<std::wstring_view> items = pg_list_to_vstring_view(list);
	auto length = get_target_string_length(items);
	wchar_t* output = new wchar_t[length];
	auto* current = output;
	std::for_each(std::rbegin(items), std::rend(items), [&current](const auto& sw) {
		current += sw.copy(current, sw.size());
		*current++ = L'.';
	});
	*--current = L'\0';
	return output;
}
#endif

#ifdef NO_STD_STRING_NO_STRING_VIEW
// Same as before, but let's find out if std::string_view introduces any real penalty to performance
// Result: difference is miniscule, it might be faster, but only for very long lists, more and better tests would need to be performed.

auto pg_list_to_vc_string(const List* const list)
{
	std::vector<const wchar_t*> items;
	items.reserve(list->length);
	for (const auto* listEl = list->head; listEl; listEl = listEl->next)
		items.push_back(get_name(listEl));
	return items;
}

auto get_target_string_length(const std::vector<const wchar_t*>& items)
{
	std::size_t length = 0;
	return std::accumulate(std::cbegin(items), std::cend(items), length, [](auto sum, const auto& s) {return sum + wcslen(s) + 1; });
}

wchar_t* reversion_function(const List* const list)
{
	auto items = pg_list_to_vc_string(list);
	auto length = get_target_string_length(items);
	wchar_t* output = new wchar_t[length];
	auto* current = output;
	std::for_each(std::rbegin(items), std::rend(items), [&current](const auto* ch) {
		while (*ch) {
			*current++ = *ch++;
		}
		*current++ = L'.';
	});
	*--current = L'\0';
	return output;
}
#endif

#ifdef RECURSIVE
// Out of curiosity, tried recursive version instead of string_view. 
// Unless the input list is big (eg. 8k cells), this should not blow the stack quickly.
// Nonetheless, the non-recursive implementations are more efficient
auto* copy_reversed_list(const ListCell* cell, wchar_t* dest) {
	if (!cell)
		return dest;
	dest = copy_reversed_list(cell->next, dest);
	const auto* ch = get_name(cell);
	while (*ch) {
		*dest++ = *ch++;
	}
	*dest++ = L'.';
	return dest;
};

wchar_t* reversion_function(const List* const list)
{
	auto length = 0;
	for (const auto* listEl = list->head; listEl; listEl = listEl->next)
		length += wcslen(get_name(listEl)) + 1;

	wchar_t* output = new wchar_t[length];
	auto* end = copy_reversed_list(list->head, output);
	*--end = L'\0';
	return output;
}
#endif

#ifdef RECURSIVE_LAMBDA
#include <functional>

// Out of curiosity, tried recursive version instead of string_view. 
// Unless the input list is really big, this should not blow the stack quickly.
// Unfortunately I didn't find the way to get rid of std::function, which lowers the performance
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



