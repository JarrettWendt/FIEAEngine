#pragma once
#include "LibMath.h"

namespace Library::Math
{
	template<std::unsigned_integral T>
	void Decrement(T& t, T diff) noexcept
	{
		t -= t ? diff : 0;
	}
}
