#pragma once
#include "Entity.h"

namespace Library
{
#pragma region Properties
	constexpr size_t Entity::NumChildren() const noexcept
	{
		return children.Size();
	}
	
	constexpr bool Entity::HasChildren() const noexcept
	{
		return NumChildren() > 0;
	}
	
	constexpr const std::string& Entity::GetName() const noexcept
	{
		return name;
	}
#pragma endregion
	
	template<typename Derived>
	inline std::shared_ptr<Derived> Entity::CreateChild(const std::string& childName)
	{
		return Adopt(childName, std::make_shared<Derived>());
	}
	
	template<typename Derived>
	inline std::shared_ptr<Derived> Entity::Adopt(const std::string& childName, std::shared_ptr<Derived> child)
	{
		ThrowName(childName);

		if (child->Parent() != shared_from_this())
		{
			const auto [it, inserted] = children.Insert(childName, std::make_shared<Derived>());
			if (!inserted) [[unlikely]]
			{
				throw InvalidNameException("child with name " + childName + " already exists");
			}
			auto ret = it->value;
			ret->name = childName;
			ret->parent = shared_from_this();
			return ret;
		}
		// already a child
		return child;
	}
	
	template<typename Derived>
	inline std::shared_ptr<Derived> Entity::Adopt(std::shared_ptr<Derived> child) noexcept
	{
		return Adopt(child->name, child);
	}
}
