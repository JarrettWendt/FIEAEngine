// MIT License Copyright (c) 2020 Jarrett Wendt

#pragma once

/**
 * Namespace for constants with the intent of `using namespace` without polluting the global namespace with unwanted symbols.
 */
namespace Library::Math::Constants
{
	// these constants were retrieved from WolframAlpha
	constexpr long double PI_llf = 3.141592653589793238462643383279502884197169399375105820974L;
	constexpr double PI_lf = PI_llf;
	constexpr float PI_f = PI_llf;
	constexpr float PI = PI_llf;

	// tau is 2pi
	constexpr long double TAU_llf = 6.283185307179586476925286766559005768394338798750211641949L;
	constexpr double TAU_lf = TAU_llf;
	constexpr float TAU_f = TAU_llf;
	constexpr float TAU = TAU_llf;

	// eta is pi/2
	constexpr long double ETA_llf = 1.570796326794896619231321691639751442098584699687552910487L;
	constexpr double ETA_lf = ETA_llf;
	constexpr float ETA_f = ETA_llf;
	constexpr float ETA = ETA_llf;
}

namespace Library::Math
{
	// aliases to Library::Math::Constants for when you don't want to `using namespace` and the fully qualified namespace is too wordy.
	using PI_llf = Constants::PI_llf;
	using PI_lf = Constants::PI_lf;
	using PI_f = Constants::PI_f;
	using PI = Constants::PI;

	using TAU_llf = Constants::TAU_llf;
	using TAU_lf = Constants::TAU_lf;
	using TAU_f = Constants::TAU_f;
	using TAU = Constants::TAU;

	using ETA_llf = Constants::ETA_llf;
	using ETA_lf = Constants::ETA_lf;
	using ETA_f = Constants::ETA_f;
	using ETA = Constants::ETA;
	
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
