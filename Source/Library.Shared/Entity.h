#pragma once
#include "Attributed.h"

namespace Library
{
	[[Reflectable]];
	class Entity : public Attributed
	{
		ATTRIBUTED_DECLARATIONS(Attributed)
		ATTRIBUTED_SPECIAL_MEMBERS(Entity, default)
		friend class Engine;

	private:
		[[Attribute]]
		Transform localTransform{};

		/** memoized transform relative to the parent */
		Transform relativeTransform{};
		
		[[Attribute]]
		std::string name{ "Entity" };

		[[Attribute]]
		bool enabled{ true };

		/** whether the relativeTransform is good */
		bool transformInval{ true };

#pragma region Properties
		[[nodiscard]] constexpr bool& Enabled() noexcept;
		[[nodiscard]] constexpr bool Enabled() const noexcept;
		
		[[nodiscard]] std::shared_ptr<Entity> Parent() noexcept;
		[[nodiscard]] std::shared_ptr<const Entity> Parent() const noexcept;
		
		[[nodiscard]] std::shared_ptr<Entity> Child(const std::string& childName) noexcept;
		[[nodiscard]] std::shared_ptr<const Entity> Child(const std::string& childName) const noexcept;
#pragma endregion

#pragma region Insert
		template<typename Derived = Entity>
		std::shared_ptr<Derived> CreateChild(const std::string& childName = "Entity");

		template<typename Derived = Entity>
		std::shared_ptr<Derived> Adopt(const std::string& childName, std::shared_ptr<Derived> child);

		template<typename Derived = Entity>
		std::shared_ptr<Derived> Adopt(std::shared_ptr<Derived> child) noexcept;
#pragma endregion

		virtual void Init();
		
		virtual void Update();
	};
}

#include "Entity.inl"
