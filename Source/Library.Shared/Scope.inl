#pragma once
#include "Scope.h"

namespace Library
{
	inline constexpr size_t Scope::Size() const noexcept
	{
		return map.Size();
	}

	inline constexpr bool Scope::IsEmpty() const noexcept
	{
		return map.IsEmpty();
	}

	inline constexpr Scope* Scope::Parent() const noexcept
	{
		return parent;
	}

	inline constexpr const std::string& Scope::NameInParent() const noexcept
	{
		return nameInParent;
	}
}
