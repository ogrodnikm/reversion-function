#pragma once

#include <algorithm>
#include <numeric>
#include <vector>
#include <string_view>
#include <tuple>


template<typename T>
using ReversibleContainer = std::vector<T>;

template<typename V>
using get_char_t = std::remove_cv_t<std::remove_pointer_t<std::remove_cv_t<typename V::value_type>>>;

template<typename Container>
using is_bidirectional_container = std::is_base_of<std::bidirectional_iterator_tag, typename Container::iterator::iterator_category>;

template<typename T>
static inline const auto& get_string(const T& s) noexcept
{
	return s;
}

template<typename CharT>
static inline auto build_string_view(const std::basic_string_view<CharT>& str) noexcept
{
	return str;
}

template<typename CharT>
static inline auto build_string_view(const std::basic_string<CharT>& str) noexcept
{
	return std::basic_string_view<CharT>(str.c_str(), str.size());
}

template<typename CharT>
static inline auto build_string_view(const CharT* str) noexcept
{
	return std::basic_string_view<CharT>(str);
}

template<typename CharT>
static inline auto copy_string_to_buffer(const CharT* ch, CharT* buffer) noexcept
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
static inline void set_last_buffer_value(CharT* buffer) noexcept
{
	*buffer = 0;
}

template<typename Iter, typename CharT>
static inline void copy_reversed_into_buffer(Iter begin, Iter end, CharT* buffer)
{
	std::for_each(begin, end, [&buffer](const auto& ch) {
		buffer = copy_string_to_buffer(get_string(ch), buffer);
	});
	set_last_buffer_value(--buffer);
}

template<typename Iter>
static inline auto get_target_string_length(Iter begin, Iter end)
{
	std::size_t length = 0;
	return std::accumulate(begin, end, length, [](auto sum, const auto& s) {
		return sum + build_string_view(get_string(s)).size() + 1;
	});
}

template<typename CharT, typename Iter>
static inline auto copy_reversed_into_new_buffer(Iter begin, Iter end)
{
	auto length = get_target_string_length(begin, end);
	if (length == 0)
	{
		CharT* output = new CharT[1];
		*output = 0;
		return output;
	}
	CharT* output = new CharT[length];
	copy_reversed_into_buffer(begin, end, output);
	return output;
}

template<typename Container>
static inline auto get_iterators(const Container& cont)
{
	if constexpr (is_bidirectional_container<Container>::value)
		return std::make_tuple(std::rbegin(cont), std::rend(cont));
	else
		return std::make_tuple(std::cbegin(cont), std::cend(cont));
}

template<typename Container>
static inline auto copy_reversed_into_new_buffer(const Container& cont)
{
	auto[begin, end] = get_iterators(cont);
	if constexpr (std::is_arithmetic_v<get_char_t<Container>>)
	{
		return copy_reversed_into_new_buffer<get_char_t<Container>>(begin, end);
	}
	else
	{
		return copy_reversed_into_new_buffer<get_char_t<get_char_t<Container>>>(begin, end);
	}
}

template<typename Container>
static inline auto container_to_reversible(const Container& cont)
{
	using character_type = get_char_t<Container>;
	ReversibleContainer<std::basic_string_view<character_type>> items;
	items.reserve(cont.size());
	for (auto& element : cont) {
		items.push_back(build_string_view(get_string(element)));
	}
	return items;
}

template<typename Container>
static inline auto* reversion_function(const Container& cont)
{
	if constexpr (is_bidirectional_container<Container>::value)
	{
		return copy_reversed_into_new_buffer(cont);
	}
	else
	{
		auto items = container_to_reversible(cont);
		return copy_reversed_into_new_buffer(items);
	}
}

template<typename Container>
static inline auto* reversion_function(Container& cont)
{
	if constexpr(std::is_const_v<typename Container::iterator::value_type> ||
		is_bidirectional_container<Container>::value)
	{
		return reversion_function(static_cast<std::add_const_t<Container>>(cont));
	}
	else
	{
		cont.reverse();
		return copy_reversed_into_new_buffer(cont);
	}
}
