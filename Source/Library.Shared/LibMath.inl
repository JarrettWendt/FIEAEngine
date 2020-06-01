// MIT License Copyright (c) 2020 Jarrett Wendt

#pragma once
#include "LibMath.h"

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
}
