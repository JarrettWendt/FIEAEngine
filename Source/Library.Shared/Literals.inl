// MIT License Copyright (c) 2020 Jarrett Wendt

#pragma once
#include "Literals.h"

namespace Library::Literals
{
	inline constexpr std::size_t operator""_z(const unsigned long long int x)
	{
		return std::size_t(x);
	}

	inline constexpr std::ptrdiff_t operator""_ptrdiff(const unsigned long long int x)
	{
		return ptrdiff_t(x);
	}
}
