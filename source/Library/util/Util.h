// MIT License Copyright (c) 2020 Jarrett Wendt

#pragma once

#include <sstream>		// std::stringstream
#include <iterator>		// std::output_iterator

#include "Concept.h"

namespace Library::Concept
{
	template<typename T>
	concept ReserveStrategy = requires(T t, size_t size, size_t capacity)
	{
		{ t(size, capacity) }->std::convertible_to<size_t>;
	};
}

namespace Library::Util
{
	/** A static empty string for functions that want to return empty string by const& */
	inline static const std::string EmptyString{ "" };
	
	struct DefaultReserveStrategy final
	{
		size_t operator()(size_t size, size_t capacity) noexcept;
	};
	
	struct PrimeReserveStrategy final
	{
		size_t operator()(size_t size, size_t capacity) noexcept;
	};

	/**
	 * For when you can't be bothered with any cast semantics.
	 * Uses a union to force the cast.
	 *
	 * @param <To>		destination type
	 * @param <From>	source type
	 * @param f			source value
	 * @returns			bytes of f casted to a To
	 */
	template<typename To, typename From>
	To UnionCast(From f) noexcept;

	/**
	 * @param <T>	type to look for
	 * @param <Ts>	set of variadic templates to query
	 * @returns		whether or not T is in Ts
	 */
	template<typename T, typename... Ts>
	constexpr bool IsOneOf() noexcept;

	/**
	 * @returns		whether or not T is convertible to any of Ts
	 */
	template<typename T, typename... Ts>
	constexpr bool IsConvertible() noexcept;

	/**
	 * @returns		whether or not T is nothrow convertible to any of Ts
	 */
	template<typename T, typename... Ts>
	constexpr bool IsNothrowConvertible() noexcept;

	/**
	 * @returns the zero-based index of T in Ts
	 * @returns std::numeric_limits<size_t>::max() if T is not in Ts
	 */
	template <typename T, typename U, typename... Ts>
	constexpr std::size_t Index() noexcept;

	/**
	 * static_assert(false) inside a constexpr-if inside a template will fail to compile.
	 * The static_assert must depend on the template type.
	 * So instead use static_assert(Util::DependentFalse<Ts...>)
	 */
	template<typename...>
	constexpr std::false_type DependentFalse{};

#pragma region Query
	/**
	 * performs a linear query over the range using a predicate
	 * 
	 * @param <Range>				range type
	 * @param <UnaryPredicate>		predicate type
	 * @param range					any range
	 * @param predicate				a unary predicate taking a Range::value_type
	 * @returns						iterator to the first element matching the predicate
	 */
	template<std::ranges::range Range, std::predicate<typename Range::value_type> UnaryPredicate>
	[[nodiscard]] auto Find(Range& range, UnaryPredicate predicate)
	{
		auto it = range.begin();
		while (it != range.end() && !predicate(*it))
		{
			++it;
		}
		return it;
	}

	/**
	 * performs a linear query over the range using a predicate
	 *
	 * @param <Range>				range type
	 * @param <UnaryPredicate>		predicate type
	 * @param range					any range
	 * @param predicate				a unary predicate taking a Range::value_type
	 * @returns						iterator to the first element matching the predicate
	 */
	template<std::ranges::range Range, std::predicate<typename Range::value_type> UnaryPredicate>
	[[nodiscard]] auto Find(const Range& range, UnaryPredicate predicate)
	{
		return Find(const_cast<Range&>(range), predicate);
	}

	/**
	 * performs a linear query over the range for t
	 *
	 * @param <Range>				range type
	 * @param <T>					element type
	 * @param range					any range
	 * @param t						an element that may or may not be in range
	 * @returns						iterator to the first element matching t
	 */
	template<std::ranges::range Range, typename T>
	[[nodiscard]] auto Find(Range& range, const T& t)
	{
		return Find(range, [&t](const auto& a) { return a == t; });
	}

	/**
	 * performs a linear query over the range for t
	 *
	 * @param <Range>				range type
	 * @param <T>					element type
	 * @param range					any range
	 * @param t						an element that may or may not be in range
	 * @returns						iterator to the first element matching t
	 */
	template<std::ranges::range Range, typename T>
	[[nodiscard]] auto Find(const Range& range, const T& t)
	{
		return Find(const_cast<Range&>(range), t);
	}

	/**
	 * performs a linear query over the range using a predicate
	 *
	 * @param <Range>				range type
	 * @param <UnaryPredicate>		predicate type
	 * @param range					any range
	 * @param predicate				a unary predicate taking a Range::value_type
	 * @returns						whether or not an element satisfies the predicate
	 */
	template<std::ranges::range Range, std::predicate<typename Range::value_type> UnaryPredicate>
	[[nodiscard]] bool Exists(const Range& range, UnaryPredicate predicate)
	{
		return Find(range, predicate) != range.end();
	}

	/**
	 * performs a linear query over the range for t
	 *
	 * @param <Range>				range type
	 * @param <T>					element type
	 * @param range					any range
	 * @param t						value that may or may not be in range
	 * @returns						whether or not t is in range
	 */
	template<std::ranges::range Range, typename T>
	[[nodiscard]] bool Contains(const Range& range, const T& t)
	{
		return Find(range, t) != range.end();
	}

	template<std::ranges::range Range, std::predicate<typename Range::value_type> UnaryPredicate>
	[[nodiscard]] bool AllOf(const Range& range, UnaryPredicate predicate)
	{
		return std::all_of(range.begin(), range.end(), predicate);
	}

	template<std::ranges::range Range, typename T, typename BinaryOperation>
	[[nodiscard]] T Accumulate(const Range& range, T init, BinaryOperation op)
	{
		return std::accumulate(range.begin(), range.end(), init, op);
	}
#pragma endregion

#pragma region SFINAE
	// UNDER CONSTRUCTION: This works but is ugly. Concepts can replace this.
	namespace
	{
		template<class Left, class Right>
		struct HasOperatorEqualImplementation
		{
			template<class U, class V>
			static auto Test(U*)->decltype(std::declval<U>() == std::declval<V>());
			template<typename, typename>
			static auto Test(...)->std::false_type;

			using value = typename std::is_same<bool, decltype(Test<Left, Right>(0))>::type;
		};
	}
	
	/*
	 * @param <Left>	lhs of operation
	 * @param <Right>	rhs of operation
	 * @returns		whether or not T::operator==(EqualTo) is defined
	 */
	template<class Left, class Right = Left>
	inline static constexpr bool HasOperatorEqual = typename HasOperatorEqualImplementation<Left, Right>::value();
	
	/**
	 * @returns whether or not T is defined (it could be forward declared).
	 */
	template<typename T>
	inline static constexpr bool IsDefined = sizeof(T) > 0;

	namespace
	{
		template<typename T, typename U, typename... Ts>
		struct FirstConvertibleImpl
		{
			using type = std::conditional_t<std::is_convertible_v<T, U>, U, typename FirstConvertibleImpl<T, Ts...>::type>;
		};

		template<typename T, typename U>
		struct FirstConvertibleImpl<T, U>
		{
			using type = U;
		};
	}

	/**
	 * @returns the first type in Ts that T is convertible to 
	 */
	template<typename T, typename... Ts>
	using FirstConvertible = typename FirstConvertibleImpl<T, Ts...>::type;

	namespace
	{
		template<typename T, typename U, typename... Ts>
		struct FirstNothrowConvertibleImpl
		{
			using type = std::conditional_t<std::is_nothrow_convertible_v<T, U>, U, typename FirstNothrowConvertibleImpl<T, Ts...>::type>;
		};

		template<typename T, typename U>
		struct FirstNothrowConvertibleImpl<T, U>
		{
			using type = U;
		};
	}

	/**
	 * @returns the first type in Ts that T is nothrow convertible to
	 */
	template<typename T, typename... Ts>
	using FirstNothrowConvertible = typename FirstNothrowConvertibleImpl<T, Ts...>::type;

	/**
	 * Looks in Ts for a type matching T.
	 * If T itself is in Ts, returns T.
	 * If T is nothrow convertible to a value in Ts, returns the first of those values.
	 * If T is convertible to a value in Ts, returns the first of those values.
	 * If T is not convertible to any value in Ts, static_asserts.
	 */
	template<typename T, typename... Ts>
	using BestMatch = std::conditional_t<IsOneOf<T, Ts...>(), T, std::conditional_t<IsNothrowConvertible<T, Ts...>(), FirstNothrowConvertible<T, Ts...>, FirstConvertible<T, Ts...>>>;
#pragma endregion

#pragma region String
	/**
	 * @returns true	if str is empty
	 * @returns true	if str is comprised of only whitespace
	 * @returns false	otherwise
	 */
	bool IsEmptyOrWhitespace(const std::string& str) noexcept;

	/**
	 *
	 * @returns			whether str is comprised of only whitespace
	 * @returns false	if str is empty
	 */
	bool IsWhitespace(const std::string& str) noexcept;

	/**
	 *
	 * @returns			whether or not str has any alpha characters
	 */
	bool HasAlpha(const std::string& str) noexcept;

	/**
	 * @returns			str with all alpha characters lowercase
	 */
	std::string ToLower(std::string str) noexcept;

	/**
	 * @returns			str with all alpha characters uppercase
	 */
	std::string ToUpper(std::string str) noexcept;

	/**
	 *
	 * @returns			whether or not all alpha characters are lowercase
	 * @returns false	if str has no alpha characters
	 */
	bool IsLower(const std::string& str) noexcept;

	/**
	 *
	 * @returns			whether or not all alpha characters are uppercase
	 * @returns false	if str has no alpha characters
	 */
	bool IsUpper(const std::string& str) noexcept;

	/**
	 * @returns			str with no whitespace
	 */
	std::string RemoveWhitespace(std::string str) noexcept;

	// TODO: Figure out how to overload operator<< for all ranges.
	// The problem is that operator<< becomes ambiguous for predefined types like std::string which are already ranges.
	template<std::forward_iterator It>
	std::ostream& StreamTo(std::ostream& stream, It first, It last);

	/**
	 * @param <T>	the type to stringify
	 * @param t		the value to stringify
	 * @returns		the stringification of t
	 */
	template<typename T>
	std::string ToString(const T& t) noexcept;

	/**
	 * @param <T>	the type to stringify
	 * @param t		the value to stringify
	 * @returns		the stringification of t
	 */
	template<typename T>
	std::wstring ToWString(const T& t) noexcept;

	/**
	 * @param str		string to look through
	 * @param from		substring to replace
	 * @param to		replacement
	 * @returns newly created string with replaced values
	 */
	std::string ReplaceAll(std::string str, const std::string &from, const std::string &to);
	
	/**
	 * @param filepath	a file path with mixed or incorrect '/' or '\\' based on platform
	 * @returns a file path with directory separators fixed based on platform
	 */
	std::string FixDirectorySeparators(std::string filepath) noexcept;

	/**
	 * Converts a path with a Windows drive letter to WSL notation.
	 * For example: "C:" to "/mnt/c"
	 *
	 * @param absfilepath	an absolute filepath
	 * @returns	the path with the drive letter converted to WSL notation 
	 */
	std::string WindowsToWSLDir(std::string absfilepath) noexcept;
#pragma endregion

	/**
	 * Invokes either the .size() or .Size() of the passed type.
	 *
	 * @param c		a class with a .Size() or .size().
	 */
	template<Concept::HasSize Class>
	constexpr size_t GetSize(const Class& c) noexcept;
}

namespace std
{
	/**
	 * std::to_string overload which takes two iterators.
	 * std::ostream& operator<<(std::ostream&, const It::value_type&) must be defined.
	 *
	 * @param <It>		iterator type
	 * @param first		the beginning iterator
	 * @param last		the ending iterator
	 */
	template<std::forward_iterator It>
	[[nodiscard]] std::string to_string(const It first, const It last)
	{
		std::stringstream stream;
		Library::Util::StreamTo(stream, first, last);
		return stream.str();
	}

	/**
	 * std::to_string overload which takes any type of range.
	 *
	 * @param <Range>	the type of range
	 * @param range		the range
	 */
	template<std::ranges::range Range>
	[[nodiscard]] std::string to_string(const Range& range)
	{
		using namespace std;
		return to_string(range.begin(), range.end());
	}

#pragma warning(push)
#pragma warning(disable: 4505)		// Unreferenced local function has been removed.
	[[nodiscard]] static std::string to_string(const char c)
	{
		return std::string(1, c);
	}
	
	[[nodiscard]] static std::string to_string(const wchar_t c)
	{
		return to_string(char(c));
	}

	[[nodiscard]] constexpr const std::string& to_string(const std::string& str)
	{
		return str;
	}

	[[nodiscard]] static std::string to_string(const std::wstring& str)
	{
#pragma warning(push)
#pragma warning(disable: 4244)	// possible loss of data
		return std::string(str.begin(), str.end());
#pragma warning(pop)
	}

	[[nodiscard]] static std::string to_string(const wchar_t* str)
	{
		return std::to_string(std::wstring(str));
	}

	[[nodiscard]] static std::wstring to_wstring(const std::string& str)
	{
		return std::wstring(str.begin(), str.end());
	}
#pragma warning(pop)

	template<typename First, typename Last>
	[[nodiscard]] std::string to_string(const std::pair<First, Last>& pair)
	{
		std::stringstream stream;
		stream << "{ " << std::to_string(pair.first) << ", " << std::to_string(pair.second) << " }";
		return stream.str();
	}

	template<typename First, typename Last>
	std::ostream& operator<<(std::ostream& stream, const std::pair<First, Last>& pair)
	{
		stream << "{ " << pair.first << ", " << pair.second << " }";
		return stream;
	}
}

#include "Util.inl"
