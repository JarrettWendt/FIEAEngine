// MIT License Copyright (c) 2020 Jarrett Wendt

#pragma once
#include "Scope.h"

namespace Library
{
#pragma region Properties
	inline constexpr size_t Scope::Size() const noexcept
	{
		return map.Size();
	}

	inline constexpr bool Scope::IsEmpty() const noexcept
	{
		return map.IsEmpty();
	}
#pragma endregion
}
