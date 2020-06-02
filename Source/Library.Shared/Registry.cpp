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
	
	Registry::Attribute& Registry::Find(const RTTI::IDType id, const std::string& name)
	{
		return *Util::Find(registry.At(id).attributes, [&name](const Attribute& a) { return a.name == name; });
	}
	
	//bool Registry::Contains(const RTTI::IDType id) noexcept
	//{
	//	return registry.Contains(id);
	//}
	//
	//bool Registry::Contains(const RTTI::IDType id, const std::string& name) noexcept
	//{
	//	if (const auto it = registry.Find(id))
	//	{
	//		return Util::Contains(it->value.attributes, Attribute{});
	//	}
	//	return false;
	//}
}
