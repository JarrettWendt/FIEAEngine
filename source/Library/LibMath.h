// MIT License Copyright (c) 2020 Jarrett Wendt

#pragma once

/**
 * Namespace for constants with the intent of `using namespace` without polluting the global namespace with unwanted symbols.
 */
namespace Library::Math::Constants
{
	// these constants were retrieved from WolframAlpha
	constexpr long double PI_llf = 3.141592653589793238462643383279502884197169399375105820974L;
	constexpr double PI_lf = double(PI_llf);
	constexpr float PI_f = float(PI_llf);
	constexpr float PI = float(PI_llf);

	// tau is 2pi
	constexpr long double TAU_llf = 6.283185307179586476925286766559005768394338798750211641949L;
	constexpr double TAU_lf = double(TAU_llf);
	constexpr float TAU_f = float(TAU_llf);
	constexpr float TAU = float(TAU_llf);

	// eta is pi/2
	constexpr long double ETA_llf = 1.570796326794896619231321691639751442098584699687552910487L;
	constexpr double ETA_lf = double(ETA_llf);
	constexpr float ETA_f = float(ETA_llf);
	constexpr float ETA = float(ETA_llf);
}

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
	 * @param n		a whole number
	 * @returns		whether or not this number is prime.
	 */
	bool IsPrime(size_t n);

	/**
	 * O(n)			where n = retval - num		most cases
	 * O(n^2)		where n = retval			if the sieve must be regenerated
	 *
	 * @param num	a whole number
	 * @returns		the next prime > n
	 */
	size_t NextPrime(size_t num);

	/**
	 * re-maps a number from one rage to another
	 *
	 * @param x			the number to map
	 * @param inMin		the lower bound of x's current range
	 * @param inMax		the upper bound of x's current range
	 * @param outMin	the lower bound of x's target range
	 * @param outMax	the upper bound of x's target range
	 */
	template<typename T>
	T ReMap(T x, T inMin, T inMax, T outMin, T outMax) noexcept;

	/**
	 * re-maps a number from one range to [0, 1]
	 *
	 * @param x			the number to map
	 * @param inMin		the lower bound of x's current range
	 * @param inMax		the upper bound of x's current range
	 */
	template<typename T>
	T ReMap01(T x, T inMin, T inMax) noexcept;

	/**
	 * mathematically conformant signed modulo
	 * 
	 * in mathematics:
	 * -1 mod 10 == -1
	 * 1 mod -10 == -9
	 * 
	 * in C/C++:
	 * -1 % 10 == 9
	 * 1 % -10 == 1
	 */
	template<typename T, typename M>
	auto Mod(T t, M m) noexcept;
}

#include "LibMath.inl"
