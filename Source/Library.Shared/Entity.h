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

		using SharedEntity = std::shared_ptr<Entity>;
		
	private:
		Transform localTransform{};

		/** memoized transform relative to the parent */
		Transform relativeTransform{};

		using MapType = HashMap<std::string, SharedEntity>;
		MapType children;
		
		[[Attribute]]
		std::string name{ "Entity" };
		
		/** non-owning reference to parent */
		std::weak_ptr<Entity> parent{};

		[[Attribute]]
		bool enabled{ true };

		/** whether the relativeTransform is good */
		bool transformInval{ true };

#pragma region iterator
		class const_iterator;

		class iterator final
		{
			friend class Entity;
			friend class const_iterator;

		public:
			using iterator_category = std::forward_iterator_tag;
			using value_type = SharedEntity;
			using difference_type = ptrdiff_t;
			using pointer = Entity*;
			using reference = value_type&;

		private:
			MapType::iterator it;

			iterator(MapType::iterator it) noexcept;

		public:
			SPECIAL_MEMBERS(iterator, default)

#pragma region Operators
			/**
			 * Post-increment
			 * Does nothing if alrady at end().
			 * O(1)
			 *
			 * @returns			the iterator before the increment
			 *
			 * @asserts			this iterator is initialized
			 */
			iterator operator++(int) noexcept;

			/**
			 * Pre-increment
			 * Does nothing if alrady at end().
			 * O(1)
			 *
			 * @returns			the iterator after the increment
			 *
			 * @asserts			this iterator is initialized
			 */
			iterator& operator++() noexcept;

			/**
			 * @returns		The value of the index the iterator is at.
			 *
			 * @asserts		this iterator is initialized and not at end().
			 */
			[[nodiscard]] reference operator*() const;

			/**
			 * @returns		The value of the index the iterator is at.
			 *
			 * @asserts		this iterator is initialized and not at end().
			 */
			[[nodiscard]] pointer operator->() const;

			/**
			 * @param other		the iterator to compare against
			 * @returns			whether or not they are equivalent
			 */
			[[nodiscard]] bool operator==(const iterator& other) const noexcept;

			/**
			 * @param other		the iterator to compare against
			 * @returns			whether or not they are inequivalent
			 */
			[[nodiscard]] bool operator!=(const iterator& other) const noexcept;

			/**
			 * @returns true if this iterator is both initialized and not at end().
			 */
			[[nodiscard]] operator bool() const noexcept;

			/**
			 * Inverse of operator bool().
			 *
			 * @returns true if this iterator is uninitialized or at end(), false otherwise.
			 */
			[[nodiscard]] bool operator!() const noexcept;
#pragma endregion

			/**
			 * O(1)
			 * @returns		Whether or not this iterator is at end().
			 *
			 * @asserts		the iterator is initialized.
			 */
			[[nodiscard]] bool IsAtEnd() const noexcept;

			/**
			 * O(1)
			 * @returns		Whether or not this iterator is at begin().
			 */
			[[nodiscard]] bool IsAtBegin() const noexcept;
		};

		class const_iterator final
		{
			friend class Entity;

			CONST_FORWARD_ITERATOR(const_iterator, iterator)

			/**
			 * @returns true if this iterator is both initialized and not at end().
			 */
			[[nodiscard]] operator bool() const noexcept;

			/**
			 * Inverse of operator bool().
			 *
			 * @returns true if this iterator is uninitialized or at end(), false otherwise.
			 */
			[[nodiscard]] bool operator!() const noexcept;

			/**
			 * O(1)
			 * @returns		Whether or not this iterator is at end().
			 *
			 * @throws std::runtime_error	if the iterator is uninitialized.
			 */
			[[nodiscard]] bool IsAtEnd() const noexcept;

			/**
			 * O(1)
			 * @returns		Whether or not this iterator is at begin().
			 */
			[[nodiscard]] bool IsAtBegin() const noexcept;
		};

		BEGIN_END(iterator, const_iterator, Entity)
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
