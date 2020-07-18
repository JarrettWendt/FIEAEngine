#pragma once
#include "InternedString.h"

namespace Library
{
	constexpr size_t String::Length() const noexcept
	{
		return intern->string.length();
	}
	
	constexpr bool String::IsEmpty() const noexcept
	{
		return intern->string.empty();
	}
}
