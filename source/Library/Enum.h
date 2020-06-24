// MIT License Copyright (c) 2020 Jarrett Wendt

#pragma once

#include <iterator>
#include <exception>

#include "Concept.h"
#include "Macros.h"
#include "LibMath.h"	// Mod

namespace Library
{
	/// <summary>
	/// Template Enum class that exists for it's Iterator.
	/// The Iterator can iterate through any enum class provided that they provide:
	/// Begin, which should be the first element in the enum.
	/// End, which should be the last (not last + 1) element in the enum.
	///
	/// TODO: It might be better if begin/end were lowercase
	/// TODO: It might be a good idea to require some sort of T::count/size
	/// </summary>
	template<Concept::Enumeration T>
	class Enum final
	{
	public:
		using underlying_type = typename std::underlying_type_t<T>;
		static inline size_t Count = underlying_type(T::End) - underlying_type(T::Begin) + 1;
		
		class iterator final
		{
		public:
			using size_type = std::size_t;
			using difference_type = std::ptrdiff_t;
			using value_type = T;
			using reference = T;
			using pointer = T*;
			using iterator_category = std::forward_iterator_tag;

			iterator(T value);
			SPECIAL_MEMBERS(iterator, default)

			T operator*() const;
			iterator& operator++();
			iterator operator++(int);

			bool operator==(const iterator& rhs) const;
			bool operator!=(const iterator& rhs) const;

			operator bool() const noexcept;

		private:
			T value;
		};

		iterator begin() const;
		iterator end() const;

		/// <summary>
		/// Provided templated mechanism for converting an enum to a string.
		/// Specialize this and implement it however you wish.
		/// It is suggested to use a static local array that is indexed into.
		/// </summary>
		static const std::string& ToString(T t);

		/// <summary>
		/// Attempts to parse a string as an enum.
		/// Up to the user if the method throws an exception or returns a sentinel value if the string cannot be parsed.
		/// It is suggested to use a static local HashMap that is indexed into.
		/// </summary>
		/// <param name="str">String to parse</param>
		/// <returns>Enum of that string</returns>
		static T FromString(const std::string& str);
	};

	template<Concept::Enumeration T>
	std::ostream& operator<<(std::ostream& stream, const T t)
	{
		return stream << Enum<T>::ToString(t);
	}
}

namespace std
{
	template<Library::Concept::Enumeration T>
	[[nodiscard]] std::string to_string(const T t)
	{
		return Library::Enum<T>::ToString(t);
	}
}

/**
 * Macro which defines the following operators for the passed enum type:
 * operator+
 * operator-
 * operator++ (pre/post)
 * operator-- (pre/post)
 *
 * These operators will overflow/underflow within the range of [Begin, End]
 * Assumes [Begin, End] is a positive contiguous number space
 *
 * @param EnumType		an enum type
 */
#define ENUM_OPERATORS(EnumType)																			\
__pragma(warning(push))																						\
__pragma(warning(disable:4244)) /* conversion, possible loss of data */										\
inline EnumType operator+(EnumType e, std::underlying_type_t<EnumType> i)									\
{																											\
	auto a = decltype(i)(e) + i;																			\
	a = Library::Math::Mod(a, decltype(i)(EnumType::End) - decltype(i)(EnumType::Begin) + 1);				\
	a += decltype(i)(EnumType::Begin);																		\
	return EnumType(a);																						\
}																											\
inline EnumType operator-(const EnumType e, std::underlying_type_t<EnumType> i)								\
{																											\
	auto a = decltype(i)(e) - i;																			\
	a = Library::Math::Mod(a, decltype(i)(EnumType::End) - decltype(i)(EnumType::Begin) + 1);				\
	a += decltype(i)(EnumType::Begin);																		\
	return EnumType(a);																						\
}																											\
inline EnumType& operator+=(EnumType& e, const std::underlying_type_t<EnumType> i)							\
{																											\
	return e = operator+(e, i);																				\
}																											\
inline EnumType& operator-=(EnumType& e, const std::underlying_type_t<EnumType> i)							\
{																											\
	return e = operator-(e, i);																				\
}																											\
inline EnumType& operator++(EnumType& e)																	\
{																											\
	return e = operator+(e, 1);																				\
}																											\
inline EnumType operator++(EnumType& e, int)																\
{																											\
	const auto ret = e;																						\
	operator++(e);																							\
	return ret;																								\
}																											\
inline EnumType& operator--(EnumType& e)																	\
{																											\
	return e = operator-(e, 1);																				\
}																											\
inline EnumType operator--(EnumType& e, int)																\
{																											\
	const auto ret = e;																						\
	operator--(e);																							\
	return ret;																								\
}																											\
__pragma(warning(pop))

#include "Enum.inl"
