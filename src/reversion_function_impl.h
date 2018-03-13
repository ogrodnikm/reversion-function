#pragma once

#include <algorithm>
#include <numeric>
#include <vector>

template<typename T>
using ReversibleContainer = std::vector<T>;

template<typename CharT>
static inline auto copy_string_to_buffer(const CharT* ch, CharT* buffer)
{
	while (*ch) {
		*buffer++ = *ch++;
	}
	*buffer++ = L'.';
	return buffer;
}

template<typename CharT>
static inline auto copy_string_to_buffer(const std::basic_string_view<CharT>& string, CharT* buffer)
{
	buffer += string.copy(buffer, string.size());
	*buffer++ = L'.';
	return buffer;
}

template<typename CharT>
static inline auto copy_string_to_buffer(const std::basic_string<CharT>& string, CharT* buffer)
{
	return copy_string_to_buffer(std::basic_string_view<CharT>(string.c_str(), string.size()), buffer);
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
	ReversibleContainer<List::value_type> items;
	items.reserve(list.size());
	for (auto& element : list) {
		items.push_back(List::get_string(element));
	}
	return items;
}

template<typename CharT, typename Container>
static inline auto get_target_string_length(const Container& items)
{
	// TODO: string_type must be supported by std::basic_string_view

	std::size_t length = 0;
	return std::accumulate(std::cbegin(items), std::cend(items), length, [](auto sum, const auto& s) {
		return sum + std::basic_string_view<CharT>(s).size() + 1;
	});
}

template<typename V>
using get_char_t_ = std::remove_cv_t<std::remove_pointer_t<std::remove_cv_t<typename V::value_type>>>;

template<typename V>
using get_char_t = std::conditional_t<std::is_arithmetic_v<get_char_t_<V>>, get_char_t_<V>, get_char_t_<get_char_t_<V>>>;

template<typename Container>
using is_bidirectional_container = std::is_base_of<std::bidirectional_iterator_tag, typename Container::iterator::iterator_category>;

template<typename CharT, typename Container>
auto copy_reversed_into_new_buffer(const Container& cont)
{
	using character_type = get_char_t_<Container>;
	auto length = get_target_string_length<CharT>(cont);
	CharT* output = new CharT[length];
	copy_reversed_into_buffer(cont, output);
	return output;
}

template<typename Container>
auto copy_reversed_into_new_buffer(const Container& cont)
{
	if constexpr (std::is_arithmetic_v<get_char_t_<Container>>)
	{
		return copy_reversed_into_new_buffer<get_char_t_<Container>>(cont);
	}
	else
	{
		return copy_reversed_into_new_buffer<get_char_t_<get_char_t_<Container>>>(cont);
	}
}

template<typename List>
static inline auto* reversion_function(const List& list)
{
	// TODO: different types of buffer (e.g. regular string)
	if constexpr (is_bidirectional_container<List>::value)
	{
		return copy_reversed_into_new_buffer(list);
	}
	else
	{
		auto items = list_to_reversible(list); // provide reversible container instead of list
		return copy_reversed_into_new_buffer(items);
	}
}