// MIT License Copyright (c) 2020 Jarrett Wendt

#pragma once
#include "Attributed.h"

namespace Library
{
	template<typename T>
	inline T* Attributed::ByteOffsetThis(size_t byteOffset) const noexcept
	{
		return reinterpret_cast<T*>(reinterpret_cast<uint8_t*>(const_cast<Attributed*>(this)) + byteOffset);
	}
}
