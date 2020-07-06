// MIT License Copyright (c) 2020 Jarrett Wendt

#include "pch.h"
#include "Registry.h"

namespace Library
{
	size_t Registry::NumAttributes(const RTTI::IDType id) noexcept
	{
		if (const auto it = registry.Find(id))
		{
			auto& [base, attributes, num] = it->value;
			if (num == std::numeric_limits<size_t>::max()) [[unlikely]]
			{
				num = attributes.Size() + NumAttributes(base);
			}
			return num;
		}
		return 0;
	}
}
