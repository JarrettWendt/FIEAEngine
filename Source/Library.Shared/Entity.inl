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
	
	template<std::derived_from<Entity> Derived, typename... Args>
	inline std::shared_ptr<Derived> Entity::CreateChild(const std::string& childName, Args&&... args)
	{
		return Adopt(childName, std::make_shared<Derived>(std::forward<Args>(args)...));
	}

	template<std::derived_from<Entity> Derived, typename ...Args>
	inline std::shared_ptr<Derived> Entity::CreateChild(std::string&& childName, Args&& ...args)
	{
		return Adopt(std::move(childName), std::make_shared<Derived>(std::forward<Args>(args)...));
	}
	
	template<std::derived_from<Entity> Derived, typename ...Args>
	inline std::shared_ptr<Derived> Entity::CreateChild(Args&& ...args)
	{
		const auto child = std::make_shared<Derived>(std::forward<Args>(args)...);
		return Adopt(child->GetName(), child);
	}
}
