// MIT License Copyright (c) 2020 Jarrett Wendt

#pragma once
#include "Random.h"

namespace Library::Random
{
	template<std::forward_iterator It, typename Engine>
	typename std::iterator_traits<It>::reference Element(It first, It last)
	{
		using difference_type = typename std::iterator_traits<It>::difference_type;

		static Engine rng;
		const auto range = std::distance(first, last) - 1;

		if constexpr (std::_Is_random_iter_v<It>)
		{
			std::uniform_int_distribution<difference_type> dist(0, range);
			return *(first + dist(rng));
		}
		else if constexpr (std::is_pointer_v<It>)
		{
			std::uniform_int_distribution<difference_type> dist(0, range);
			return *(first + dist(rng));
		}
		else // regular forward iterator
		{
			std::uniform_int_distribution<difference_type> dist(0, range);
			const size_t index = dist(rng);
			for (size_t i = 0; i < index; i++, ++first);
			return *first;
		}
	}

	template<std::ranges::input_range Range, typename Engine>
	typename Range::reference Element(Range& r)
	{
		return Element(r.begin(), r.end());
	}

	template<std::ranges::range Range, typename Engine>
	const typename Range::reference Element(const Range& r)
	{
		return Element(const_cast<Range&>(r));
	}

	template<Concept::Arithmetic T, typename Engine>
	T Next()
	{
		return Range<T, Engine>(T(0), std::numeric_limits<T>::max());
	}

	template<Concept::Arithmetic T, typename Engine>
	T Range(const T min, T max)
	{
		static Engine rng;
		if constexpr (std::is_floating_point_v<T>)
		{
			std::uniform_real_distribution<T> dist(min, max);
			return dist(rng);
		}
		else if constexpr (std::is_same_v<bool, T>)
		{
			std::uniform_int_distribution<short> dist(min, max);
			return bool(dist(rng));
		}
		else if constexpr (std::is_same_v<char, T>)
		{
			std::uniform_int_distribution<short> dist(min, max);
			return char(dist(rng));
		}
		else if constexpr (std::is_integral_v<T>)
		{
			std::uniform_int_distribution<T> dist(min, max);
			return dist(rng);
		}
		else
		{
			// The above cases should cover every arithmetic type so this in an unexpected state.
			static_assert(false, "unexpected type");
		}
	}

	template<Concept::Enumeration T, typename Engine>
	T Next()
	{
		return Range<T, Engine>(T::Begin, T::End);
	}

	template<Concept::Enumeration T, typename Engine>
	T Range(const T min, const T max)
	{
		using base = typename std::underlying_type<T>::type;
		return T(Range<base, Engine>(min, max));
	}
}
