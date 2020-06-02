#pragma once
#include "Attributed.h"

namespace Library
{
	[[Reflectable]];
	class Entity : public Attributed, public std::enable_shared_from_this<Entity>
	{
		ATTRIBUTED_DECLARATIONS(Attributed)
		ATTRIBUTED_SPECIAL_MEMBERS(Entity, default)
		friend class Engine;

	private:
		Transform localTransform{};

		/** memoized transform relative to the parent */
		Transform relativeTransform{};

		HashMap<std::string, std::shared_ptr<Entity>> children;
		
		[[Attribute]]
		std::string name{ "Entity" };
		
		/** non-owning reference to parent */
		std::weak_ptr<Entity> parent{};

		[[Attribute]]
		bool enabled{ true };

		/** whether the relativeTransform is good */
		bool transformInval{ true };

#pragma region iterator

#pragma endregion

#pragma region Properties
		[[nodiscard]] constexpr bool& Enabled() noexcept;
		[[nodiscard]] constexpr bool Enabled() const noexcept;
		
		[[nodiscard]] std::shared_ptr<Entity> Parent() noexcept;
		[[nodiscard]] std::shared_ptr<const Entity> Parent() const noexcept;
		
		[[nodiscard]] std::shared_ptr<Entity> Child(const std::string& childName) noexcept;
		[[nodiscard]] std::shared_ptr<const Entity> Child(const std::string& childName) const noexcept;
#pragma endregion

#pragma region Insert
		/**
		 * Appends a default constructed Derived type.
		 * O(1)
		 *
		 * @param name		name for this child
		 * @returns			reference to newly appended child
		 *
		 * @throws InvalidNameException
		 */
		template<typename Derived = Entity>
		std::shared_ptr<Derived> CreateChild(const std::string& childName = "Entity");

		/**
		 * Reparents the passed Entity to this one.
		 * O(1)
		 *
		 * @param name		the name for the adoptee
		 * @param child		the Entity to become a child of this one
		 * @returns			a reference back to the passed Entity for chaining
		 *
		 * @throws InvalidNameException
		 */
		template<typename Derived = Entity>
		std::shared_ptr<Derived> Adopt(const std::string& childName, std::shared_ptr<Derived> child);

		/**
		 * Reparents the passed Entity to this one.
		 * Will use the child's existing name.
		 * O(1)
		 *
		 * @param child		the Entity to become a child of this one
		 * @returns			a reference back to the passed Entity for chaining
		 *
		 * @throws InvalidNameException
		 */
		template<typename Derived = Entity>
		std::shared_ptr<Derived> Adopt(std::shared_ptr<Derived> child) noexcept;
#pragma endregion

#pragma region Remove
		/**
		 * Orphans this Entity from its parent.
		 * Does nothing if it is already an orphan.
		 * O(1)
		 */
		void Orphan() noexcept;
#pragma endregion

		virtual void Init();
		
		virtual void Update();
	};
}

#include "Entity.inl"
