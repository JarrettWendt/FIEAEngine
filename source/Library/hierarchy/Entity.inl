#pragma once
#include "Entity.h"
#include "Input.h"

namespace Library
{
#pragma region wrappers
#pragma region float
	template <CoordinateSpace Space, Transform::Component Type, size_t Index, bool IsConst>
	Entity::FloatWrapper<Space, Type, Index, IsConst>::FloatWrapper(Entity& owner) noexcept :
		owner(owner) {}

	template<CoordinateSpace Space, Transform::Component Type, size_t Index, bool IsConst>
	inline Entity::FloatWrapper<Space, Type, Index, IsConst>::operator float() const noexcept
	{
		return owner.GetTransform<Space>().template GetComponent<Type>()[Index];
	}
#pragma endregion
	
#pragma region Vector3
	template <CoordinateSpace Space, Transform::Component Type, bool IsConst>
	Entity::Vector3Wrapper<Space, Type, IsConst>::Vector3Wrapper(Entity& owner) noexcept :
		owner(owner) {}
	
	template <CoordinateSpace Space, Transform::Component Type, bool IsConst>
	inline Entity::Vector3Wrapper<Space, Type, IsConst>::operator Vector3() const noexcept
	{
		const auto trans = owner.GetTransform<Space>();
		if constexpr (Type == Transform::Component::Translation)
		{
			return trans.translation;
		}
		else
		{
			return trans.scale;
		}
	}	
	
	template <CoordinateSpace Space, Transform::Component Type, bool IsConst>
	Entity::FloatWrapper<Space, Type, 0, IsConst> Entity::Vector3Wrapper<Space, Type, IsConst>::X() const noexcept
	{
		return owner;
	}

	template <CoordinateSpace Space, Transform::Component Type, bool IsConst>
	Entity::FloatWrapper<Space, Type, 1, IsConst> Entity::Vector3Wrapper<Space, Type, IsConst>::Y() const noexcept
	{
		return owner;
	}
	
	template <CoordinateSpace Space, Transform::Component Type, bool IsConst>
	Entity::FloatWrapper<Space, Type, 2, IsConst> Entity::Vector3Wrapper<Space, Type, IsConst>::Z() const noexcept
	{
		return owner;
	}
#pragma endregion
	
#pragma region Quaternion
	template<CoordinateSpace Space, bool IsConst>
	Entity::QuaternionWrapper<Space, IsConst>::QuaternionWrapper(Entity& owner) noexcept :
		owner(owner) {}
	
	template<CoordinateSpace Space, bool IsConst>
	Entity::QuaternionWrapper<Space, IsConst>::operator Quaternion() const noexcept
	{
		const auto trans = owner.GetTransform<Space>();
		return trans.rotation;
	}
	
	template<CoordinateSpace Space, bool IsConst>
	Entity::FloatWrapper<Space, Transform::Component::Rotation, 0, IsConst> Entity::QuaternionWrapper<Space, IsConst>::X() const noexcept
	{
		return owner;
	}
	
	template<CoordinateSpace Space, bool IsConst>
	Entity::FloatWrapper<Space, Transform::Component::Rotation, 1, IsConst> Entity::QuaternionWrapper<Space, IsConst>::Y() const noexcept
	{
		return owner;
	}
	
	template<CoordinateSpace Space, bool IsConst>
	Entity::FloatWrapper<Space, Transform::Component::Rotation, 2, IsConst> Entity::QuaternionWrapper<Space, IsConst>::Z() const noexcept
	{
		return owner;
	}
	
	template<CoordinateSpace Space, bool IsConst>
	Entity::FloatWrapper<Space, Transform::Component::Rotation, 3, IsConst> Entity::QuaternionWrapper<Space, IsConst>::W() const noexcept
	{
		return owner;
	}
#pragma endregion
	
#pragma region Transform
	template<CoordinateSpace Space, bool IsConst>
	Entity::TransformWrapper<Space, IsConst>::TransformWrapper(Entity& owner) noexcept :
		owner(owner) {}
	
	template<CoordinateSpace Space, bool IsConst>
	Entity::TransformWrapper<Space, IsConst>::operator Library::Transform() const noexcept
	{
		return owner.template GetTransform<Space>();
	}
	
	template<CoordinateSpace Space, bool IsConst>
	Entity::Vector3Wrapper<Space, Transform::Component::Translation, IsConst> Entity::TransformWrapper<Space, IsConst>::Translation() noexcept
	{
		return owner;
	}
	
	template<CoordinateSpace Space, bool IsConst>
	Entity::Vector3Wrapper<Space, Transform::Component::Scale, IsConst> Entity::TransformWrapper<Space, IsConst>::Scale() noexcept
	{
		return owner;
	}
	
	template<CoordinateSpace Space, bool IsConst>
	Entity::QuaternionWrapper<Space, IsConst> Entity::TransformWrapper<Space, IsConst>::Rotation() noexcept
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
	inline const Transform& Entity::GetTransform() const noexcept
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
	Entity::TransformWrapper<Space, false> Entity::Transform() noexcept
	{
		return *this;
	}
	
	template <CoordinateSpace Space>
	Entity::TransformWrapper<Space, true> Entity::Transform() const noexcept
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
