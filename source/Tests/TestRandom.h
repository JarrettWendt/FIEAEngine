#pragma once
#include "Random.h"

namespace Library::Random
{
	template<std::ranges::range Range, typename Engine = DefaultEngine>
	[[nodiscard]] Range Next(const size_t size = 10)
	{
		using value_type = typename Range::value_type;
		std::vector<value_type> v;
		v.reserve(size);
		for (size_t i = 0; i < size; i++)
		{
			v.push_back(Next<value_type>());
		}
		return { v.begin(), v.end() };
	}

	template<std::ranges::range Range, typename Engine = DefaultEngine>
	[[nodiscard]] typename Range::value_type NotIn(const Range& range)
	{
		using value_type = typename Range::value_type;
		value_type ret;
		do
		{
			ret = Next<value_type>();
		} while (Util::Contains(range, ret));
		return ret;
	}

	template<std::ranges::range Range, typename Engine = DefaultEngine>
	[[nodiscard]] Range Unique(const size_t size)
	{
		using value_type = typename Range::value_type;
		std::vector<value_type> v;
		v.reserve(size);
		for (size_t i = 0; i < size; i++)
		{
			v.push_back(NotIn(v));
		}
		return { v.begin(), v.end() };
	}

	template<typename T, typename Engine = DefaultEngine>
	[[nodiscard]] T NotEqualTo(const T& t)
	{
		T ret;
		do
		{
			ret = Next<T>();
		} while (ret == t);
		return ret;
	}
}
