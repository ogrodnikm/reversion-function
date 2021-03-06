#include "pg_list_utils.h"
#include "reversion_function.h"
#include <vector>
#include <numeric>
#include <algorithm>
#include <string>
#include <chrono>
#include <iostream>


// TODO inependent of char type, container
auto pg_list_to_vstring_view(const List* const list)
{
	std::vector<std::wstring_view> items;
	items.reserve(list->length);
	for (const auto* listEl = list->head; listEl; listEl = listEl->next)
		items.emplace_back(get_name(listEl));
	return items;
}

// This is first implementation that came up in my mind, very inefficient, but clear
wchar_t* reversion_function_basic(const List* const list)
{
	std::vector<std::wstring_view> items = pg_list_to_vstring_view(list);
	auto reversed = std::accumulate(std::rbegin(items), std::rend(items), std::wstring(), [](auto& s, const auto& sw) {
		s += sw;
		s += L'.';
		return s;
	});
	wchar_t* output = new wchar_t[reversed.size()];
	wcscpy(output, reversed.c_str());
	output[reversed.size() - 1] = L'\0';
	return output;
}

auto get_target_string_length(const std::vector<std::wstring_view>& items)
{
	std::size_t length = 0;
	return std::accumulate(std::cbegin(items), std::cend(items), length, [](auto sum, const auto& s) {return sum + s.size() + 1; });
}

// Similar to simple implementation, but performance should be much better, since there are not std::string allocations
wchar_t* reversion_function_no_string(const List* const list)
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


// Same as before, but let's find out if std::string_view introduces any real penalty to performance
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

wchar_t* reversion_function_no_string_view(const List* const list)
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

wchar_t* reversion_function_recursive(const List* const list)
{
	auto length = 0;
	for (const auto* listEl = list->head; listEl; listEl = listEl->next)
		length += wcslen(get_name(listEl)) + 1;

	wchar_t* output = new wchar_t[length];
	auto* end = copy_reversed_list(list->head, output);
	*--end = L'\0';
	return output;
}

#include <functional>
// Out of curiosity, tried recursive version instead of string_view. 
// Unless the input list is really big, this should not blow the stack quickly.
// Unfortunately I didn't find the way to get rid of std::function, which lowers the performance
wchar_t* reversion_function_recursive_functional(const List* const list)
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
		while (*ch) {
			*current++ = *ch++;
		}
		*current++ = L'.';
	};
	fun(list->head);
	*--current = L'\0';
	return output;
}


// Reverse the list. Most C-like implementation. 
// Fastest method, since there is no temporary data needed, but requires List muatation 
// (although this also can be solved by reversing the list again).
wchar_t* reversion_function_reverse_list(List* list)
{
	list_reverse(*list);

	std::size_t length = 0;
	for (const auto* cell = list->head; cell; cell = cell->next)
		length += wcslen(get_name(cell)) + 1;

	wchar_t* output = new wchar_t[length];
	auto* current = output;
	for (const auto* cell = list->head; cell; cell = cell->next)
	{
		// This might be faster if memcpy is used, but would require the knowledge of string length
		const wchar_t* ch = get_name(cell);
		while (*ch) {
			*current++ = *ch++;
		}
		*current++ = L'.';
	}

	*--current = L'\0';
	return output;
}

// Same as before, but let's try to be pseudo const correct
wchar_t* reversion_function_reverse_list_twice(const List* list)
{
	auto* out = reversion_function_reverse_list(const_cast<List*>(list));
	list_reverse(*const_cast<List*>(list));
	return out;
}

template<typename Func>
void invoke(Func&& func, const char* name)
{
	// Of course it might be better to use specialized tools than this simple method
	auto t1 = std::chrono::high_resolution_clock::now();
	for (auto i = 0; i < 100; ++i) {
		func();
	}
	auto t2 = std::chrono::high_resolution_clock::now();
	std::cout << "test function " << name << " took "
		<< std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1).count()
		<< " milliseconds\n";
}

int main()
{
	ListWrapper list;
	for (auto i = 0; i < 100000; ++i) {
		list_append(list, { L"MyApp", L"MyModule", L"MyTable" });
	}

	// invoke([&] { reversion_function_basic(&list); }, "reversion_function_basic"); // this is much too slow
	// invoke([&] { reversion_function_recursive(&list); }, "reversion_function_recursive"); // stack overflow
	// invoke([&] { reversion_function_recursive_functional(&list); }, "reversion_function_recursive_functional"); // stack overflow - no tail recursion optimization yet :(

	invoke([&] { auto* out = reversion_function_no_string_view(&list); delete[]out; }, "reversion_function_no_string_view");
	invoke([&] { auto* out = reversion_function_reverse_list_twice(&list); delete[]out; }, "reversion_function_reverse_list_twice");
	invoke([&] { auto* out = reversion_function_no_string(&list); delete[]out;  }, "reversion_function_no_string");
	invoke([&] { auto* out = reversion_function_reverse_list(&list); delete[]out; }, "reversion_function_reverse_list");

	/* Results:
	- reversion_function_reverse_list and reversion_function_no_string are the fastest with similar results
	- those approaches will be used in real implementation
	*/

	invoke([&] { auto* out = reversion_function(static_cast<const List*>(&list)); delete[]out; }, "reversion_function const");
	invoke([&] { auto* out = reversion_function(&list); delete[]out; }, "reversion_function");

	/* Implementation is similar as fast to prototype implementation (within measurement margin error), 
	   with added flexibility of input parameters. */
	getchar();

}

