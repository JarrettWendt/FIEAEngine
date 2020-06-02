#include "pch.h"
#include "Entity.h"

namespace Library
{
	constexpr bool& Entity::Enabled() noexcept
	{
		return enabled;
	}
	
	constexpr bool Entity::Enabled() const noexcept
	{
		return const_cast<Entity*>(this)->Enabled();
	}

	std::shared_ptr<Entity> Entity::Parent() noexcept
	{
		return parent.lock();
	}

	std::shared_ptr<const Entity> Entity::Parent() const noexcept
	{
		return const_cast<Entity*>(this)->Parent();
	}

	std::shared_ptr<Entity> Entity::Child(const std::string& childName) noexcept
	{
		const auto it = children.Find(childName);
		return it ? it->value : nullptr;
	}

	std::shared_ptr<const Entity> Entity::Child(const std::string& childName) const noexcept
	{
		return const_cast<Entity*>(this)->Child(childName);
	}
	
	void Entity::Orphan() noexcept
	{
		auto p = Parent();
		if (p)
		{
			p->children.Remove(name);
			parent = {};
		}
	}

	void Entity::Init()
	{
		// TODO: invoke all children Init
	}

	void Entity::Update()
	{
		// TODO: invoke all children Update
	}
}
