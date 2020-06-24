// MIT License Copyright (c) 2020 Jarrett Wendt

#pragma once

#include <iterator>
#include <random>
#include <ranges>

#include "Macros.h"
#include "Concept.h"

namespace Library::Random
{
	using DefaultEngine = std::default_random_engine;

	/**
	 * Retrieves a random value between the two iterators in the range [first, last)
	 * At most O(1) if It is a random_access_iterator.
	 * At least O(n) otherwise.
	 *
	 * @param <It>				the type of iterator to accept
	 * @param <Engine>			the random engine to use
	 * @param first				the beginning iterator to read from (inclusive)
	 * @param last				the ending iterator to read from (exclusive)
	 * @returns					a random element in the range of the two iterators
	 */
	template<std::forward_iterator It, typename Engine = DefaultEngine>
	[[nodiscard]] typename std::iterator_traits<It>::reference Element(It first, It last);

	/**
	 * @param <Range>			the type of range to query
	 * @param <Engine>			the random engine to use
	 * @param r					the range to retrieve a random element from
	 * @returns					a random element in the container
	 */
	template<std::ranges::input_range Range, typename Engine = DefaultEngine>
	[[nodiscard]] typename Range::reference Element(Range& r);

	/**
	 * @param <Range>			the type of range to query
	 * @param <Engine>			the random engine to use
	 * @param r					the range to retrieve a random element from
	 * @returns					a random element in the container
	 */
	template<std::ranges::range Range, typename Engine = DefaultEngine>
	[[nodiscard]] const typename Range::reference Element(const Range& r);

	/**
	 * Generate a number in the range [0, std::numeric_limits<T>::max()) for any arithmetic type.
	 *
	 * @param <T>		the type to randomly generate
	 * @param <Engine>	the random engine to use
	 * @returns			a random number in the range [0, max)
	 */
	template<Concept::Arithmetic T, typename Engine = DefaultEngine>
	[[nodiscard]] T Next();

	/**
	 * Generate a number in the range range [min, max) for any arithmetic type.
	 *
	 * @param <T>		the type to randomly generate
	 * @param <Engine>	the random engine to use
	 * @param min		the minimum value that can be generated (inclusive)
	 * @param max		the maximum value that can be generated (inclusive)
	 * @returns			a random number in the range [min, max]
	 */
	template<Concept::Arithmetic T, typename Engine = DefaultEngine>
	[[nodiscard]] T Range(T min, T max);

	/**
	 * Generate a random value from an enumeration.
	 * Assumes the enumeration has a Begin and End.
	 *
	 * @param <T>		enumeration type
	 * @param <Engine>	the random engine to use
	 * @returns			a random number in the range [Begin, End]
	 */
	template<Concept::Enumeration T, typename Engine = DefaultEngine>
	[[nodiscard]] T Next();
	
	/**
	 * Generate a random value from an enumeration.
	 * Assumes the enumeration has a Begin and End.
	 *
	 * @param <T>		enumeration type
	 * @param <Engine>	the random engine to use
	 * @param min		the minimum value that can be generated (inclusive)
	 * @param max		the maximum value that can be generated (inclusive)
	 * @returns			a random number in the range [min, max]
	 */
	template<Concept::Enumeration T, typename Engine = DefaultEngine>
	[[nodiscard]] T Range(T min, T max);
}

#include "Random.inl"
