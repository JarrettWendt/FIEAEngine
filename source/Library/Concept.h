// MIT License Copyright (c) 2020 Jarrett Wendt

#pragma once

#include <concepts>

namespace Library::Util
{
	/**
	 * @param <T>	type to look for
	 * @param <Ts>	set of variadic templates to query
	 * @returns		whether or not T is in Ts
	 */
	template<typename T, typename... Ts>
	static constexpr bool IsOneOf() noexcept;
}

namespace Library::Concept
{
	template<typename T>
	concept Ostreamable = requires(std::ostream stream, T t)
	{
		{ stream << t }->std::convertible_to<std::ostream&>;
	};

	template<typename T>
	concept has_std_to_string = requires(T t)
	{
		{ std::to_string(t) }->std::convertible_to<std::string>;
	};
	
	template<typename T>
	concept Enumeration = std::is_enum_v<T>;
	
	template<typename Range, typename T>
	concept RangeOf = std::ranges::range<Range> && std::is_same_v<typename Range::value_type, T>;

	template<typename T>
	concept Class = std::is_class_v<T>;

	template<typename T, typename U>
	concept Related = std::is_convertible_v<T*, U*>;

	template<typename T, typename... Ts>
	concept OneOf = Util::IsOneOf<T, Ts...>();

	template<typename T>
	concept has_size = requires(T t)
	{
		{ t.size() }->std::convertible_to<size_t>;
	};

	template<typename T>
	concept has_Size = requires(T t)
	{
		{ t.Size() }->std::convertible_to<size_t>;
	};

	template<typename T>
	concept HasSize = has_size<T> || has_Size<T>;

	template<typename T>
	concept has_value_type = requires { typename T::value_type; };

	// TODO: <experimental/ranges/iterator> has this
	template<typename It>
	concept Iterator = std::_Is_iterator_v<It>;

	template<typename Left, typename Right = Left>
	concept LessThanComparable = requires(Left l, Right r)
	{
		{ l < r }->std::convertible_to<bool>;
	};

	template<typename T>
	concept Arithmetic = std::is_arithmetic_v<T>;
}
