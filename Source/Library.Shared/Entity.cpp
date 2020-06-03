#include "pch.h"
#include "Entity.h"

namespace Library
{
#pragma region iterator
#pragma region non-const
	Entity::iterator::iterator(const MapType::iterator it) noexcept :
		it(it) {}
	
	Entity::iterator Entity::iterator::operator++(int) noexcept
	{
		const auto ret = *this;
		operator++();
		return ret;
	}
	
	Entity::iterator& Entity::iterator::operator++() noexcept
	{
		it.operator++();
		return *this;
	}

	Entity::iterator::reference Entity::iterator::operator*() const
	{
		return it->value;
	}

	Entity::iterator::pointer Entity::iterator::operator->() const
	{
		return &*operator*();
	}

	bool Entity::iterator::operator==(const iterator& other) const noexcept
	{
		return it == other.it;
	}

	bool Entity::iterator::operator!=(const iterator& other) const noexcept
	{
		return !operator==(other);
	}

	Entity::iterator::operator bool() const noexcept
	{
		return it;
	}

	bool Entity::iterator::operator!() const noexcept
	{
		return !it;
	}

	bool Entity::iterator::IsAtBegin() const noexcept
	{
		return it.IsAtBegin();
	}

	bool Entity::iterator::IsAtEnd() const noexcept
	{
		return it.IsAtEnd();
	}
#pragma endregion

#pragma region const
	Entity::const_iterator::operator bool() const noexcept
	{
		return it;
	}

	bool Entity::const_iterator::operator!() const noexcept
	{
		return !it;
	}

	bool Entity::const_iterator::IsAtBegin() const noexcept
	{
		return it.IsAtBegin();
	}

	bool Entity::const_iterator::IsAtEnd() const noexcept
	{
		return it.IsAtEnd();
	}
#pragma endregion

Entity::iterator Entity::begin() noexcept
{
	return children.begin();
}
	
Entity::iterator Entity::end() noexcept
{
	return children.end();
}
#pragma endregion
	
#pragma region Properties
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
#pragma endregion
	
	void Entity::SetName(const std::string& newName) noexcept
	{
		if (auto p = Parent())
		{
			p->children.Remove(name);
			name = newName;
			p->children.Insert(name, shared_from_this());
		}
	}
	
#pragma region Remove
	void Entity::Orphan() noexcept
	{;
		if (auto p = Parent())
		{
			p->children.Remove(name);
			parent = {};
		}
	}

	void Entity::RemoveChild(const std::string& childName) noexcept
	{
		if (const auto it = children.Find(childName))
		{
			it->value->parent = {};
			children.Remove(it);
		}
	}
#pragma endregion

	void Entity::Init()
	{
		for (const auto& e : *this)
		{
			e->Init();
		}
	}

	void Entity::Update()
	{
		for (const auto& e : *this)
		{
			e->Update();
		}
	}
}
