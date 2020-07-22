// MIT License Copyright (c) 2020 Jarrett Wendt

#include "pch.h"
#include "Entity.h"
#include "Engine.h"
#include "InternedString.h"

using namespace Library::Literals;

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
		return it->second;
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
	SharedPtr<Entity> Entity::Parent() noexcept
	{
		return parent;
	}

	SharedPtr<const Entity> Entity::Parent() const noexcept
	{
		return const_cast<Entity*>(this)->Parent();
	}

	SharedPtr<Entity> Entity::Child(const String& childName) noexcept
	{
		if (const auto it = children.find(childName); it != children.end())
		{
			return it->second;
		}
		return nullptr;
	}

	SharedPtr<const Entity> Entity::Child(const String& childName) const noexcept
	{
		return const_cast<Entity*>(this)->Child(childName);
	}
#pragma endregion
	
	void Entity::SetName(const String& newName) noexcept
	{
		if (auto p = Parent())
		{
			p->children.erase(name);
			p->children.emplace(newName, SharedFromThis());
		}
		name = newName;
	}

#pragma region Insert	
	SharedPtr<Entity> Entity::Adopt(const String& childName, SharedEntity child)
	{
		ThrowName(childName);

		if (child->Parent() != SharedFromThis())
		{
			const auto [it, inserted] = children.emplace(childName, child);
			if (!inserted) [[unlikely]]
			{
				throw InvalidNameException("child with name "_s + childName + " already exists"_s);
			}
			assert(child == it->second);
			child->name = childName;
			child->parent = WeakFromThis();
		}
		return child;
	}

	SharedPtr<Entity> Entity::Adopt(const SharedEntity child)
	{
		return Adopt(child->name, child);
	}
#pragma endregion
	
#pragma region Remove	
	void Entity::Orphan() noexcept
	{
		if (auto p = Parent())
		{
			p->children.erase(name);
			parent = {};
		}
	}

	void Entity::RemoveChild(const String& childName) noexcept
	{
		if (const auto it = children.find(childName); it != children.end())
		{
			it->second->parent = {};
			children.erase(it);
		}
	}
#pragma endregion

#pragma region operators
	bool operator==(const Entity& a, const Entity& b) noexcept
	{
		return operator==(static_cast<const Attributed&>(a), static_cast<const Attributed&>(b));
	}

	bool operator!=(const Entity& a, const Entity& b) noexcept
	{
		return !operator==(a, b);
	}
#pragma endregion

#pragma region Helpers
	void Entity::InvalTransform() noexcept
	{
		transformInval = true;
		InvalChildTransforms();
	}

	void Entity::InvalChildTransforms() noexcept
	{
		for (auto& e : *this)
		{
			e->InvalTransform();
		}
	}
#pragma endregion
}
