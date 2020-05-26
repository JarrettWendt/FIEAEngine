// MIT License Copyright (c) 2020 Jarrett Wendt

#pragma once

namespace Library::Math
{
	/**
	 * wrapper for safely decrementing an unsigned type without underflowing
	 *
	 * @param t		value to decrement
	 * @param diff	(optional) how much to decrement by
	 */
	template<std::unsigned_integral T>
	void Decrement(T& t, T diff = 1) noexcept;
	
	/**
	 * O(1)			most cases
	 * O(n^2)		if the sieve must be regenerated
	 *
	 * @param num	a whole number
	 * @returns		whether or not this number is prime.
	 */
	bool IsPrime(size_t n);

	/**
	 * O(n)			where n = reval - num	most cases
	 * O(n^2)		where n = retval		if the sieve must be regenerated
	 *
	 * @param num	a whole number
	 * @returns		the next prime > n
	 */
	size_t NextPrime(size_t num);
}

#include "LibMath.inl"
