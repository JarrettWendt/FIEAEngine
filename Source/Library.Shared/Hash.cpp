#include "pch.h"
#include "Hash.h"

namespace Library
{
	hash_t HashUtils::AdditiveHash(const uint8_t* bytes, const size_t count) noexcept
	{
		hash_t ret{};
		for (size_t i = 0; i < count; i++)
		{
			ret += prime * bytes[i];
		}
		return ret;
	}
}
