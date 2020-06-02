// MIT License Copyright (c) 2020 Jarrett Wendt

#pragma once
#include "Registry.h"

namespace Library
{
	template<typename Invokable>
	inline void Registry::ForEach(const RTTI::IDType id, Invokable func)
	{
		if (const auto it = registry.Find(id))
		{
			const auto& [baseID, attributes, num] = it->value;
			for (const Attribute& attribute : attributes)
			{
				func(attribute);
			}
			ForEach(baseID, func);
		}
	}
}
