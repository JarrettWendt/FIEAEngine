// MIT License Copyright (c) 2020 Jarrett Wendt

#pragma once
#include "Attributed.h"

namespace Library
{
#pragma region Properties
	constexpr size_t Attributed::NumAttributes() const noexcept
	{
		return attributes.Size();
	}

	constexpr bool Attributed::HasAttributes() const noexcept
	{
		return NumAttributes() > 0;
	}
#pragma endregion
	
	template<typename T>
	inline T* Attributed::ByteOffsetThis(const size_t byteOffset) const noexcept
	{
		return reinterpret_cast<T*>(reinterpret_cast<uint8_t*>(const_cast<Attributed*>(this)) + byteOffset);
	}
}
