// MIT License Copyright (c) 2020 Jarrett Wendt

#pragma once
#include "Util.h"

namespace Library::Util
{
	template<typename To, typename From>
	To UnionCast(From f) noexcept
	{
		union
		{
			To to;
			From from;
		};
		from = f;
		return to;
	}
	
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
	
	template<typename T>
	std::string ToString(const T& t) noexcept
	{
		if constexpr (Concept::Ostreamable<T>)
		{
			std::stringstream stream;
			stream << t;
			return stream.str();
		}
		else if constexpr (Concept::has_std_to_string<T>)
		{
			return std::to_string(t);
		}
		else if constexpr (std::ranges::range<T>)
		{
			std::stringstream stream;
			StreamTo(stream, t.begin(), t.end());
			return stream.str();
		}
		else
		{
			// TODO: On linux this fails even when the function is never called
#if _WIN32
			static_assert(false, "no string conversion for type");
#endif
		}
	}
	
	template<typename T>
	std::wstring ToWString(const T& t) noexcept
	{
		return std::to_wstring(ToString(t));
	}
}
