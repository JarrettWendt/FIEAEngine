// MIT License Copyright (c) 2020 Jarrett Wendt

#pragma once

#include <iterator>
#include <exception>

#include "Concept.h"
#include "Macros.h"

namespace Library
{
	/// <summary>
	/// Template Enum class that exists for it's Iterator.
	/// The Iterator can iterate through any enum class provided that they provide:
	/// Begin, which should be the first element in the enum.
	/// End, which should be the last (not last + 1) element in the enum.
	/// </summary>
	template<Concept::Enumeration T>
	class Enum final
	{
	public:
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
}

namespace std
{
	template<Library::Concept::Enumeration T>
	[[nodiscard]] std::string to_string(const T t)
	{
		return Enum<T>::ToString(t);
	}
}

#include "Enum.inl"
