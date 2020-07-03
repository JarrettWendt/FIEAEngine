#pragma once
#include "Entity.h"
#include "Input.h"

namespace Library
{
#pragma region wrappers
#pragma region float
	template <CoordinateSpace Space, Transform::Component Type, size_t index, std::derived_from<Entity> Owner>
	Entity::FloatWrapper<Space, Type, index, Owner>::FloatWrapper(Owner& owner) noexcept :
		owner(owner) {}

	template<CoordinateSpace Space, Transform::Component Type, size_t index, std::derived_from<Entity> Owner>
	inline Entity::FloatWrapper<Space, Type, index, Owner>::operator float() const noexcept
	{
		return owner.template GetTransform<Space>().template GetComponent<Type>()[index];
	}
#pragma endregion
	
#pragma region Vector3
	template<CoordinateSpace Space, Transform::Component Type, std::derived_from<Entity> Owner>
	Entity::Vector3Wrapper<Space, Type, Owner>::Vector3Wrapper(Owner& owner) noexcept :
		owner(owner) {}
	
	template<CoordinateSpace Space, Transform::Component Type, std::derived_from<Entity> Owner>
	inline Entity::Vector3Wrapper<Space, Type, Owner>::operator Vector3() const noexcept
	{
		const auto trans = owner.template GetTransform<Space>();
		if constexpr (Type == Transform::Component::Translation)
		{
			return trans.translation;
		}
		else
		{
			return trans.scale;
		}
	}	

	template <CoordinateSpace Space, Transform::Component Type, std::derived_from<Entity> Owner>
	Entity::FloatWrapper<Space, Type, 0, Owner> Entity::Vector3Wrapper<Space, Type, Owner>::X() const noexcept
	{
		return owner;
	}

	template <CoordinateSpace Space, Transform::Component Type, std::derived_from<Entity> Owner>
	Entity::FloatWrapper<Space, Type, 1, Owner> Entity::Vector3Wrapper<Space, Type, Owner>::Y() const noexcept
	{
		return owner;
	}

	template <CoordinateSpace Space, Transform::Component Type, std::derived_from<Entity> Owner>
	Entity::FloatWrapper<Space, Type, 2, Owner> Entity::Vector3Wrapper<Space, Type, Owner>::Z() const noexcept
	{
		return owner;
	}
#pragma endregion
	
#pragma region Quaternion
	template<CoordinateSpace Space, std::derived_from<Entity> Owner>
	Entity::QuaternionWrapper<Space, Owner>::QuaternionWrapper(Owner& owner) noexcept :
		owner(owner) {}

	template<CoordinateSpace Space, std::derived_from<Entity> Owner>
	Entity::QuaternionWrapper<Space, Owner>::operator Quaternion() const noexcept
	{
		const auto trans = owner.template GetTransform<Space>();
		return trans.rotation;
	}

	template <CoordinateSpace Space, std::derived_from<Entity> Owner>
	Entity::FloatWrapper<Space, Transform::Component::Rotation, 0, Owner> Entity::QuaternionWrapper<Space, Owner>::X() const noexcept
	{
		return owner;
	}

	template <CoordinateSpace Space, std::derived_from<Entity> Owner>
	Entity::FloatWrapper<Space, Transform::Component::Rotation, 1, Owner> Entity::QuaternionWrapper<Space, Owner>::Y() const noexcept
	{
		return owner;
	}

	template <CoordinateSpace Space, std::derived_from<Entity> Owner>
	Entity::FloatWrapper<Space, Transform::Component::Rotation, 2, Owner> Entity::QuaternionWrapper<Space, Owner>::Z() const noexcept
	{
		return owner;
	}

	template <CoordinateSpace Space, std::derived_from<Entity> Owner>
	Entity::FloatWrapper<Space, Transform::Component::Rotation, 3, Owner> Entity::QuaternionWrapper<Space, Owner>::W() const noexcept
	{
		return owner;
	}
#pragma endregion
	
#pragma region Transform
	template<CoordinateSpace Space, std::derived_from<Entity> Owner>
	Entity::TransformWrapper<Space, Owner>::TransformWrapper(Owner& owner) noexcept :
		owner(owner) {}
	
	template<CoordinateSpace Space, std::derived_from<Entity> Owner>
	Entity::TransformWrapper<Space, Owner>::operator Library::Transform() const noexcept
	{
		return owner.template GetTransform<Space>();
	}
	
	template<CoordinateSpace Space, std::derived_from<Entity> Owner>
	Entity::Vector3Wrapper<Space, Transform::Component::Translation, Owner> Entity::TransformWrapper<Space, Owner>::Translation() noexcept
	{
		return owner;
	}
	
	template<CoordinateSpace Space, std::derived_from<Entity> Owner>
	Entity::Vector3Wrapper<Space, Transform::Component::Scale, Owner> Entity::TransformWrapper<Space, Owner>::Scale() noexcept
	{
		return owner;
	}
	
	template<CoordinateSpace Space, std::derived_from<Entity> Owner>
	Entity::QuaternionWrapper<Space, Owner> Entity::TransformWrapper<Space, Owner>::Rotation() noexcept
	{
		return owner;
	}
#pragma endregion
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

#pragma region Transform
	template<CoordinateSpace Space>
	inline const Library::Transform& Entity::GetTransform() const noexcept
	{
		if constexpr (Space == CoordinateSpace::Local)
		{
			return localTransform;
		}
		else
		{
			if (transformInval)
			{
				if (const auto p = Parent())
				{
					worldTransform = localTransform + p->GetTransform<CoordinateSpace::World>();
				}
				else
				{
					worldTransform = localTransform;
				}
				transformInval = false;
			}
			return worldTransform;
		}
	}
	
	template<CoordinateSpace Space>
	void Entity::SetTransform(const Library::Transform& t) noexcept
	{
		if constexpr (Space == CoordinateSpace::Local)
		{
			localTransform = t;
			InvalTransform();
		}
		else
		{
			worldTransform = t;
			if (const auto p = Parent())
			{
				localTransform = worldTransform - p->GetTransform<CoordinateSpace::World>();
			}
			else
			{
				localTransform = worldTransform;
			}
			InvalChildTransforms();
		}
	}
	
	template <CoordinateSpace Space>
	Entity::TransformWrapper<Space, Entity> Entity::Transform() noexcept
	{
		return *this;
	}
	
	template <CoordinateSpace Space>
	Entity::TransformWrapper<Space, const Entity> Entity::Transform() const noexcept
	{
		return *this;
	}
#pragma endregion

#pragma region Insert
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
#pragma endregion
}
