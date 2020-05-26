// MIT License Copyright (c) 2020 Jarrett Wendt

#pragma once
#include "Coroutine.h"

namespace Library
{
	inline constexpr size_t Coroutines::Count() noexcept
	{
		return blockCoroutines.Size() + asyncCoroutines.Size();
	}
}
