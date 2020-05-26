// MIT License Copyright (c) 2020 Jarrett Wendt

#pragma once
#include "Enum.h"

namespace Library
{
	template <Concept::Enumeration T>
	inline Enum<T>::iterator::iterator(T value) :
		value(value) {}

	template <Concept::Enumeration T>
	inline bool Enum<T>::iterator::operator==(const iterator& rhs) const
	{
		return value == rhs.value;
	}

	template <Concept::Enumeration T>
	inline bool Enum<T>::iterator::operator!=(const iterator& rhs) const
	{
		return value != rhs.value;
	}

	template <Concept::Enumeration T>
	inline T Enum<T>::iterator::operator*() const
	{
		if (value > T::End)
		{
			throw std::runtime_error("Cannot dereference end().");
		}
		return value;
	}

	template <Concept::Enumeration T>
	inline typename Enum<T>::iterator& Enum<T>::iterator::operator++()
	{
		if (value <= T::End)
		{
			value = T(static_cast<typename std::underlying_type<T>::type>(value) + 1);
		}
		return *this;
	}

	template <Concept::Enumeration T>
	inline typename Enum<T>::iterator Enum<T>::iterator::operator++(int)
	{
		iterator temp = *this;
		operator++();
		return temp;
	}

	template<Concept::Enumeration T>
	inline Enum<T>::iterator::operator bool() const noexcept
	{
		return static_cast<int>(value) <= static_cast<int>(T::End);
	}

	template <Concept::Enumeration T>
	inline typename Enum<T>::iterator Enum<T>::begin() const
	{
		return iterator(T::Begin);
	}

	template <Concept::Enumeration T>
	inline typename Enum<T>::iterator Enum<T>::end() const
	{
		return iterator(T(static_cast<int>(T::End) + 1));
	}

	template<Concept::Enumeration T>
	inline const std::string& Enum<T>::ToString(T t)
	{
		static_assert(false, "No specialization");
	}

	template<Concept::Enumeration T>
	inline T Enum<T>::FromString(const std::string& str)
	{
		static_assert(false, "No specialization");
	}
}
