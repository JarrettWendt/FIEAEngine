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
		mutable Transform worldTransform{};

		using MapType = HashMap<std::string, SharedEntity>;
		MapType children;
		
		[[Attribute]]
		std::string name{ "Entity" };
		
		/** non-owning reference to parent */
		std::weak_ptr<Entity> parent{};

		[[Attribute]]
		bool enabled{ true };

		/** whether the worldTransform needs to be updated */
		mutable bool transformInval{ true };

	public:
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
		/**
		 * O(1)
		 *
		 * @returns		whether or not this Entity is enabled (Updates will not be called when disabled)
		 */
		[[nodiscard]] constexpr bool& Enabled() noexcept;

		/**
		 * O(1)
		 *
		 * @returns		whether or not this Entity is enabled (Updates will not be called when disabled)
		 */
		[[nodiscard]] constexpr bool Enabled() const noexcept;

		/**
		 * O(1)
		 * 
		 * @returns		how many children this Entity has
		 */
		[[nodiscard]] constexpr size_t NumChildren() const noexcept;

		/**
		 * O(1)
		 * 
		 * @returns		if this Entity has any children
		 */
		[[nodiscard]] constexpr bool HasChildren() const noexcept;
		
		/**
		 * O(1)
		 * 
		 * @returns		this Entity's parent
		 */
		[[nodiscard]] SharedEntity Parent() noexcept;
		
		/**
		 * O(1)
		 *
		 * @returns		this Entity's parent
		 */
		[[nodiscard]] std::shared_ptr<const Entity> Parent() const noexcept;

		/**
		 * O(1)
		 *
		 * @param childName		the name of the child to query for
		 * @returns				this child with that name
		 */
		[[nodiscard]] SharedEntity Child(const std::string& childName) noexcept;

		/**
		 * O(1)
		 *
		 * @param childName		the name of the child to query for
		 * @returns				this child with that name
		 */
		[[nodiscard]] std::shared_ptr<const Entity> Child(const std::string& childName) const noexcept;
#pragma endregion

#pragma region Transform
		/**
		 * O(1)
		 * 
		 * @returns		this Entity's local Transform
		 */
		[[nodiscard]] constexpr const Transform& GetLocalTransform() const noexcept;

		/**
		 * O(1) most cases
		 * Potentially O(n) where n is the number of parents with invalid Transforms.
		 * Must to matrix multiplications for every iteration.
		 * 
		 * @returns		this Entity's world Transform
		 */
		[[nodiscard]] const Transform& GetWorldTransform() const noexcept;

		/**
		 * O(n) where n is the number of children.
		 * Sets a bool on all children to mark their Transforms invalid (comes into play in GetWorldTransform()).
		 * 
		 * @param t		Transform to set this local one to. 
		 */
		void SetLocalTransform(const Transform& t) noexcept;

		/**
		 * O(n) where n is the number of children.
		 * Sets a bool on all children to mark their Transforms invalid (comes into play in GetWorldTransform()).
		 *
		 * @param t		Transform to set this world one to.
		 */
		void SetWorldTransform(const Transform& t) noexcept;
#pragma endregion
		
		/**
		 * O(1)
		 * 
		 * @returns		this Entity's name
		 */
		[[nodiscard]] constexpr const std::string& GetName() const noexcept;

		/**
		 * Will require re-hashing this Entity into its parent
		 * O(1)
		 * 
		 * @param newName	new name for this Entity
		 */
		void SetName(const std::string& newName) noexcept;
		
#pragma region Insert
		/**
		 * Appends a default constructed Derived type.
		 * O(1)
		 *
		 * @param childName		name for this child
		 * @param args			arguments to forward to the Derived type's ctor.
		 * @returns				reference to newly appended child
		 *
		 * @throws InvalidNameException
		 */
		template<std::derived_from<Entity> Derived = Entity, typename ...Args>
		std::shared_ptr<Derived> CreateChild(const std::string& childName, Args&&... args);

		/**
		 * Appends a default constructed Derived type.
		 * O(1)
		 *
		 * @param childName		name for this child
		 * @param args			arguments to forward to the Derived type's ctor.
		 * @returns				reference to newly appended child
		 *
		 * @throws InvalidNameException
		 */
		template<std::derived_from<Entity> Derived = Entity, typename ...Args>
		std::shared_ptr<Derived> CreateChild(std::string&& childName, Args&&... args);
		
		/**
		 * Appends a default constructed Derived type.
		 * O(1)
		 *
		 * @param args		arguments to forward to the Derived type's ctor.
		 * @returns			reference to newly appended child
		 *
		 * @throws InvalidNameException
		 */
		template<std::derived_from<Entity> Derived = Entity, typename ...Args>
		std::shared_ptr<Derived> CreateChild(Args&&... args);

		/**
		 * Reparents the passed Entity to this one.
		 * O(1)
		 *
		 * @param childName		the name for the adoptee
		 * @param child			the Entity to become a child of this one
		 * @returns				a reference back to the passed Entity for chaining
		 *
		 * @throws InvalidNameException
		 */
		SharedEntity Adopt(const std::string& childName, SharedEntity child);

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
		SharedEntity Adopt(SharedEntity child);
#pragma endregion

#pragma region Remove
		/**
		 * Orphans this Entity from its parent.
		 * Does nothing if it is already an orphan.
		 * O(1)
		 */
		void Orphan() noexcept;

		/**
		 * Orphans child by the specified name.
		 * Does nothing if no child by that name exists.
		 * O(1)
		 * 
		 * @param childName		child to orphan
		 */
		void RemoveChild(const std::string& childName) noexcept;
#pragma endregion

#pragma region operators
		/**
		 * O(n) where n is the number of attributes
		 *
		 * @param a		rhs
		 * @param b		lhs
		 * @returns		whether these Entities are the same
		 */
		[[nodiscard]] friend bool operator==(const Entity& a, const Entity& b) noexcept;

		/**
		 * O(n) where n is the number of attributes
		 *
		 * @param a		rhs
		 * @param b		lhs
		 * @returns		whether these Entities are not the same
		 */
		[[nodiscard]] friend bool operator!=(const Entity& a, const Entity& b) noexcept;
#pragma endregion

	protected:
		/**
		 * Invokes Init() on all children.
		 */
		virtual void Init();

		/**
		 * Invokes Update() on all enabled children. 
		 */
		virtual void Update();

	private:
		/**
		 * Marks that this Entity and all of its children have invalid worldTransforms. 
		 */
		void InvalTransform() noexcept;

		/**
		 * Marks all children to have invalid worldTransforms.
		 */
		void InvalChildTransforms() noexcept;
	};
}

#include "Entity.inl"
