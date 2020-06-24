// MIT License Copyright (c) 2020 Jarrett Wendt

#include "pch.h"
#include "Entity.h"
#include "Engine.h"

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

#pragma region Transform	
	const Transform& Entity::GetWorldTransform() const noexcept
	{
		if (transformInval)
		{
			if (const auto p = Parent())
			{
				worldTransform = localTransform + p->GetWorldTransform();
			}
			else
			{
				worldTransform = localTransform;
			}
			transformInval = false;
		}
		return worldTransform;
	}

	void Entity::SetLocalTransform(const Library::Transform& t) noexcept
	{
		localTransform = t;
		InvalTransform();
	}

	void Entity::SetWorldTransform(const Library::Transform& t) noexcept
	{
		worldTransform = t;
		if (const auto p = Parent())
		{
			localTransform = worldTransform - p->GetWorldTransform();
		}
		else
		{
			localTransform = worldTransform;
		}
		InvalChildTransforms();
	}
#pragma endregion
	
	void Entity::SetName(const std::string& newName) noexcept
	{
		if (auto p = Parent())
		{
			p->children.Remove(name);
			p->children.Insert(newName, shared_from_this());
		}
		name = newName;
	}

#pragma region Insert	
	std::shared_ptr<Entity> Entity::Adopt(const std::string& childName, SharedEntity child)
	{
		ThrowName(childName);

		if (child->Parent() != shared_from_this())
		{
			const auto [it, inserted] = children.Insert(childName, child);
			if (!inserted) [[unlikely]]
			{
				throw InvalidNameException("child with name " + childName + " already exists");
			}
			assert(child == it->value);
			child->name = childName;
			child->parent = weak_from_this();
		}
		return child;
	}

	std::shared_ptr<Entity> Entity::Adopt(const SharedEntity child)
	{
		return Adopt(child->name, child);
	}
#pragma endregion
	
#pragma region Remove	
	void Entity::Orphan() noexcept
	{
		Engine::pendingOrphans.PushBack(weak_from_this());
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
			if (e->Enabled())
			{
				e->Update();
			}
		}
	}

#pragma region Helpers
	void Entity::InvalTransform() noexcept
	{
		transformInval = true;
		InvalChildTransforms();
	}

	void Entity::InvalChildTransforms() noexcept
	{
		for (const auto& e : *this)
		{
			e->InvalTransform();
		}
	}
	
	void Entity::OrphanNow() noexcept
	{
		if (auto p = Parent())
		{
			p->children.Remove(name);
			parent = {};
		}
	}
#pragma endregion
}
