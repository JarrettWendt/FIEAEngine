// MIT License Copyright (c) 2020 Jarrett Wendt

#pragma once
#include "LibMath.h"
#include <cassert>

namespace Library::Math
{
	template<std::unsigned_integral T>
	void Decrement(T& t, T diff) noexcept
	{
		t -= t ? diff : 0;
	}
	
	template<typename T>
	T ReMap(const T x, const T inMin, const T inMax, const T outMin, const T outMax) noexcept
	{
		return (x - inMin) * (outMax - outMin) / (inMax - inMin) + outMin;
	}
	
	template<typename T>
	T ReMap01(const T x, T inMin, T inMax) noexcept
	{
		return ReMap(x, inMin, inMax, T(0), T(1));
	}

	template<typename T, typename M>
	auto Mod(T t, M m) noexcept
	{
		assert(m && "modulus not defined for modulo of 0");

		const auto r = t % m;
		if constexpr (std::is_signed_v<decltype(r)> && std::is_signed_v<M>)
		{
			if ((m > 0 && r < 0) || (m < 0 && r > 0))
			{
				return r + m;
			}
		}
		return r;
	}
}
