#pragma once

#include <algorithm>
#include <numeric>
#include <vector>

template<typename T>
using ReversibleContainer = std::vector<T>;

template<typename V>
using get_char_t = std::remove_cv_t<std::remove_pointer_t<std::remove_cv_t<typename V::value_type>>>;

template<typename CharT>
static inline auto build_string_view(const std::basic_string_view<CharT>& str)
{
	return str;
}

template<typename CharT>
static inline auto build_string_view(const std::basic_string<CharT>& str)
{
	return std::basic_string_view<CharT>(str.c_str(), str.size());
}

template<typename CharT>
static inline auto build_string_view(const CharT* str)
{
	return std::basic_string_view<CharT>(str);
}

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
	return copy_string_to_buffer(build_string_view(string), buffer);
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

template<typename Container>
static inline auto container_to_reversible(const Container& cont)
{
	using character_type = get_char_t<Container>;
	ReversibleContainer<std::basic_string_view<character_type>> items;
	items.reserve(cont.size());
	for (auto& element : cont) {
		items.push_back(build_string_view(Container::get_string(element)));
	}
	return items;
}

template<typename CharT, typename Container>
static inline auto get_target_string_length(const Container& items)
{
	// TODO: optimization for types with len
	std::size_t length = 0;
	return std::accumulate(std::cbegin(items), std::cend(items), length, [](auto sum, const auto& s) {
		return sum + std::basic_string_view<CharT>(s).size() + 1;
	});
}

template<typename CharT, typename Container>
auto copy_reversed_into_new_buffer(const Container& cont)
{
	using character_type = get_char_t<Container>;
	auto length = get_target_string_length<CharT>(cont);
	CharT* output = new CharT[length];
	copy_reversed_into_buffer(cont, output);
	return output;
}

template<typename Container>
auto copy_reversed_into_new_buffer(const Container& cont)
{
	if constexpr (std::is_arithmetic_v<get_char_t<Container>>)
	{
		return copy_reversed_into_new_buffer<get_char_t<Container>>(cont);
	}
	else
	{
		return copy_reversed_into_new_buffer<get_char_t<get_char_t<Container>>>(cont);
	}
}

template<typename Container>
using is_bidirectional_container = std::is_base_of<std::bidirectional_iterator_tag, typename Container::iterator::iterator_category>;

template<typename List>
static inline auto* reversion_function(const List& list)
{
	if constexpr (is_bidirectional_container<List>::value)
	{
		return copy_reversed_into_new_buffer(list);
	}
	else
	{
		auto items = container_to_reversible(list); // provide reversible container instead of list
		return copy_reversed_into_new_buffer(items);
	}
}