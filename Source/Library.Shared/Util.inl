// MIT License Copyright (c) 2020 Jarrett Wendt

#pragma once
#include "Util.h"

namespace Library::Util
{	
	template<typename T, typename ...Ts>
	constexpr bool IsOneOf() noexcept
	{
		if constexpr ((std::is_same_v<T, Ts> || ...))
		{
			return true;
		}
		else
		{
			return false;
		}
	}

	template<typename T, typename ...Ts>
	constexpr bool IsConvertible() noexcept
	{
		if constexpr ((std::is_convertible_v<T, Ts> || ...))
		{
			return true;
		}
		else
		{
			return false;
		}
	}

	template<typename T, typename ...Ts>
	constexpr bool IsNothrowConvertible() noexcept
	{
		if constexpr ((std::is_nothrow_convertible_v<T, Ts> || ...))
		{
			return true;
		}
		else
		{
			return false;
		}
	}

	template<typename T, typename U, typename ...Ts>
	constexpr std::size_t Index() noexcept
	{
		if constexpr (!IsOneOf<T, U, Ts...>())
		{
			return std::numeric_limits<size_t>::max();
		}
		else if constexpr (std::is_same_v<T, U>)
		{
			return 0;
		}
		else
		{
			return 1 + Index<T, Ts...>();
		}
	}

	template<typename T, typename U, typename ...Ts>
	constexpr std::size_t OneIndex() noexcept
	{
		if constexpr (!IsOneOf<T, U, Ts...>())
		{
			return 0;
		}
		else if constexpr (std::is_same_v<T, U>)
		{
			return 1;
		}
		else
		{
			return 1 + OneIndex<T, Ts...>();
		}
	}

	template<Concept::HasSize Class>
	constexpr size_t GetSize(const Class& c) noexcept
	{
		if constexpr (Concept::has_size<Class>)
		{
			return std::size(c);
		}
		else if constexpr (Concept::has_Size<Class>)
		{
			return c.Size();
		}
		else
		{
			return std::distance(c.begin(), c.end());
		}
	}

	template<std::forward_iterator It>
	std::ostream& StreamTo(std::ostream& stream, It first, const It last)
	{
		stream << "{ ";
		if (first != last)
		{
			stream << *first++;
			while (first != last)
			{
				stream << ", " << *first++;
			}
		}
		stream << " }";
		return stream;
	}
}
