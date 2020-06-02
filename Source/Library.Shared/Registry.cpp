#include "pch.h"
#include "Registry.h"

namespace Library
{
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
