// MIT License Copyright (c) 2020 Jarrett Wendt

#include "pch.h"
#include "Reflection.h"

namespace Library
{
	const Reflection::ConstructorWrapper* Reflection::GetConstructor(const std::string& className) noexcept
	{
		const auto it = constructors.Find(className);
		return it ? &it->value : nullptr;
	}
}
