#pragma once

#include "EngineTime.h"

namespace Library::Literals
{	
	constexpr std::size_t operator""_z(unsigned long long int x);

	constexpr std::ptrdiff_t operator""_ptrdiff(unsigned long long int x);
}

#include "Literals.inl"
