// MIT License Copyright (c) 2020 Jarrett Wendt

#include "pch.h"
#include "Hash.h"

namespace Library
{
	hash_t HashUtils::AdditiveHash(const uint8_t* bytes, const size_t count) noexcept
	{
		// TODO: optimize this based on count to instead iterate over hash_t chunks
		hash_t ret{};
		for (size_t i = 0; i < count; i++)
		{
			ret += prime * bytes[i];
		}
		return ret;
	}
}
