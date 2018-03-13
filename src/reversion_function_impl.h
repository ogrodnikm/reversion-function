#pragma once

#include <algorithm>
#include <numeric>
#include <vector>

template<typename T>
using ReversibleContainer = std::vector<T>;


// TODO: different types of strings (std::string_view, std::string)
template<typename CharT>
static inline auto copy_string_to_buffer(const CharT* ch, CharT* buffer) {
	while (*ch) {
		*buffer++ = *ch++;
	}
	*buffer++ = L'.';
	return buffer;
}

template<typename CharT>
static inline void set_last_buffer_value(CharT* buffer)
{
	// TODO: will different types of CharT work?
	CharT zero = 0;
	*buffer = zero;
	//*--buffer = L'\0';
}

template<typename ReversibleContainer, typename CharT>
static inline void copy_reversed_into_buffer(const ReversibleContainer& items, CharT* buffer)
{
	std::for_each(std::rbegin(items), std::rend(items), [&buffer](const auto& ch) {
		buffer = copy_string_to_buffer(ch, buffer);
	});
	set_last_buffer_value(--buffer);
}

template<typename List>
static inline auto list_to_reversible(const List& list)
{
	// TODO: string_view ?
	ReversibleContainer<List::string_type> items;
	items.reserve(list.size());
	for (auto& element : list) {
		items.push_back(List::get_string(element));
	}
	return items;
}

template<typename CharT, typename Container>
static inline auto get_target_string_length(const Container& items)
{
	std::size_t length = 0;
	return std::accumulate(std::cbegin(items), std::cend(items), length, [](auto sum, const auto& s) {
		return sum + std::basic_string_view<CharT>(s).size() + 1;
	});
}

template<typename List>
static inline auto* reversion_function(const List& list)
{
	// TODO: string_type must be supported by std::basic_string_view
	// TODO: different types of buffer
	auto items = list_to_reversible(list); // provide reversible container instead of list
	auto length = get_target_string_length<List::value_type>(items);

	auto* output = new List::value_type[length];
	copy_reversed_into_buffer(items, output);
	return output;
}