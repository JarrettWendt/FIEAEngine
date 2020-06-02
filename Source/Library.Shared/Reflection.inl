#pragma once
#include "Reflection.h"

namespace Library
{
	template<typename T>
	inline std::shared_ptr<T> Reflection::Construct(const std::string& className)
	{
		const auto it = constructors.Find(className);
		return it ? std::reinterpret_pointer_cast<T>(it->value()) : nullptr;
	}
}
