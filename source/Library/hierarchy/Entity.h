// MIT License Copyright(c) 2020 Jarrett Wendt

#pragma once
#include "Attributed.h"
#include "EnableSharedFromThis.h"

namespace Library::py
{
	class EntityBinding;
}

/**
 * Defaults the ctors.
 * Deletes the move/copy semantics since an Entity must exists in a shared_ptr anyway.
 * Allows user to explicitly define a dtor.
 *
 * @param DerivedType	a type which derives from Entity
 */
#define ENTITY_CTOR_ASSIGN(DerivedType)		\
	ATTRIBUTED_CTORS(DerivedType)			\
	MOVE_COPY(DerivedType, delete)

/**
 * Defaults the ctors and virtual dtor.
 * Deletes the move/copy semantics since an Entity must exists in a shared_ptr anyway.
 *
 * @param DerivedType	a type which derives from Entity
 */
#define ENTITY_SPECIAL_MEMBERS(DerivedType)	\
	ENTITY_CTOR_ASSIGN(DerivedType)			\
	virtual ~DerivedType() = default;

namespace Library
{
	/**
	 * A Transform, Vector, Quaternion, etc. can either exists in Local or World space.
	 * It's recommended to do `using enum` with this.
	 */
	enum class CoordinateSpace : uint8_t
	{
		Local,
		World
	};
	
	[[Reflectable]];
	class Entity : public Attributed, public EnableSharedFromThis<Entity>
	{
		ATTRIBUTED_DECLARATIONS(Attributed)
		ENTITY_SPECIAL_MEMBERS(Entity)
		
		friend class Engine;
		friend py::EntityBinding;

		using SharedEntity = SharedPtr<Entity>;
		class iterator;
		class const_iterator;
		
	private:
		Transform localTransform{};

		/** memoized transform relative to the parent */
		mutable Transform worldTransform{};

		using MapType = std::unordered_map<String, SharedEntity>;
		MapType children{};

		[[Attribute]]
		String name{ "Entity" };
		
		/** non-owning reference to parent */
		WeakPtr<Entity> parent{};

		[[Attribute]]
		bool enabled{ true };

		/**
		 * Whether the worldTransform needs to be updated.
		 * Initially invalid so that we don't calculate the world Transform until we first try to get it.
		 */
		mutable bool transformInval{ true };

	public:
#pragma region wrappers
		/**
		 * @param <Space>	the CoordinateSpace this wrapper operates on
		 * @param <Type>	the component of the Transform this wrapper references
		 * @param <index>	the xyzw component
		 * @param <IsConst>	whether or not this references a const or non-const Entity
		 */
		template<CoordinateSpace Space, Transform::Component Type, size_t Index, bool IsConst = false>
		struct FloatWrapper final
		{
			friend class Entity;
		private:
			Entity& owner;

			FloatWrapper(Entity& owner) noexcept;
			
		public:
			/**
			 * Implicit conversion operator.
			 * 
			 * @returns the value this wrapper references
			 */
			operator float() const noexcept;

			FloatWrapper& operator=(float f) noexcept requires (!IsConst)
			{
				auto trans = owner.GetTransform<Space>();
				trans.template GetComponent<Type>()[std::integral_constant<size_t, Index>()] = f;
				owner.SetTransform<Space>(trans);
				return *this;
			}

			FloatWrapper& operator+=(float f) noexcept requires (!IsConst)
			{
				return *this = *this + f;
			}

			FloatWrapper& operator-=(float f) noexcept requires (!IsConst)
			{
				return *this = *this - f;
			}

			FloatWrapper& operator*=(float f) noexcept requires (!IsConst)
			{
				return *this = *this * f;
			}

			FloatWrapper& operator/=(float f) noexcept requires (!IsConst)
			{
				return *this = *this / f;
			}
		};
		
		/**
		 * @param <Space>	the CoordinateSpace this wrapper operates on
		 * @param <Type>	either Scale or Translation
		 * @param <IsConst>	whether or not this references a const or non-const Entity
		 */
		template<CoordinateSpace Space, Transform::Component Type, bool IsConst = false>
		struct Vector3Wrapper final
		{
			friend class Entity;
		private:
			Entity& owner;

			Vector3Wrapper(Entity& owner) noexcept;
			
		public:
			/**
			 * Implicit conversion operator.
			 * 
			 * @returns the value this wrapper references
			 */
			operator Vector3() const noexcept;

			/**
			 * Properly invalidates the referenced owner's Transform based on CoordinateSpace.
			 *
			 * @param v		Vector3 to reassign the one this wrapper references.
			 * @returns		this
			 */
			Vector3Wrapper& operator=(const Vector3& v) noexcept requires (!IsConst)
			{
				auto trans = owner.template GetTransform<Space>();
				trans.template GetComponent<Type>() = v;
				owner.template SetTransform<Space>(trans);
				return *this;
			}

			/**
			 * Get component at compile-time.
			 * Use with Literals::operator""_zc
			 */
			template<size_t Index>
			FloatWrapper<Space, Type, Index, IsConst> operator[](std::integral_constant<size_t, Index>) const noexcept
			{
				return owner;
			}

			FloatWrapper<Space, Type, 0, IsConst> X() const noexcept;
			FloatWrapper<Space, Type, 1, IsConst> Y() const noexcept;
			FloatWrapper<Space, Type, 2, IsConst> Z() const noexcept;

			Vector3Wrapper& operator+=(float f) noexcept requires (!IsConst)
			{
				return *this = *this + f;
			}
			
			Vector3Wrapper& operator-=(float f) noexcept requires (!IsConst)
			{
				return *this = *this - f;
			}
			
			Vector3Wrapper& operator*=(float f) noexcept requires (!IsConst)
			{
				return *this = *this * f;
			}
			
			Vector3Wrapper& operator/=(float f) noexcept requires (!IsConst)
			{
				return *this = *this / f;
			}

			Vector3Wrapper& operator+=(const Vector3& v) noexcept requires (!IsConst)
			{
				return *this = *this + v;
			}
			
			Vector3Wrapper& operator-=(const Vector3& v) noexcept requires (!IsConst)
			{
				return *this = *this - v;
			}
			
			Vector3Wrapper& operator*=(const Vector3& v) noexcept requires (!IsConst)
			{
				return *this = *this * v;
			}
			
			Vector3Wrapper& operator/=(const Vector3& v) noexcept requires (!IsConst)
			{
				return *this = *this / v;
			}
		};		

		/**
		 * @param <Space>	the CoordinateSpace this wrapper operates on
		 * @param <IsConst>	whether or not this references a const or non-const Entity
		 */
		template<CoordinateSpace Space, bool IsConst = false>
		struct QuaternionWrapper final
		{
			friend class Entity;
		private:
			Entity& owner;

			QuaternionWrapper(Entity& owner) noexcept;
			
		public:
			/**
			 * Implicit conversion operator.
			 * 
			 * @returns the value this wrapper references
			 */
			operator Quaternion() const noexcept;

			/**
			 * Properly invalidates the referenced owner's Transform based on CoordinateSpace.
			 *
			 * @param q		Quaternion to reassign the one this wrapper references.
			 * @returns		this
			 */
			QuaternionWrapper& operator=(const Quaternion& q) noexcept requires (!IsConst)
			{
				auto trans = owner.GetTransform<Space>();
				trans.rotation = q;
				owner.SetTransform<Space>(trans);
				return *this;
			}

			/**
			 * Get component at compile-time.
			 * Use with Literals::operator""_zc
			 */
			template<size_t Index>
			auto operator[](std::integral_constant<size_t, Index>) const noexcept
			{
				return FloatWrapper<Space, Transform::Component::Rotation, Index, IsConst>(owner);
			}

			FloatWrapper<Space, Transform::Component::Rotation, 0, IsConst> X() const noexcept;
			FloatWrapper<Space, Transform::Component::Rotation, 1, IsConst> Y() const noexcept;
			FloatWrapper<Space, Transform::Component::Rotation, 2, IsConst> Z() const noexcept;
			FloatWrapper<Space, Transform::Component::Rotation, 3, IsConst> W() const noexcept;
		};

		/**
		 * @param <Space>	the CoordinateSpace this wrapper operates on
		 * @param <IsConst>	whether or not this references a const or non-const Entity
		 */
		template<CoordinateSpace Space, bool IsConst = false>
		struct TransformWrapper final
		{
			friend class Entity;
		private:
			Entity& owner;

			TransformWrapper(Entity& owner) noexcept;
			
		public:			
			/**
			 * Implicit conversion operator.
			 * 
			 * @returns the value this wrapper references
			 */
			operator Transform() const noexcept;

			/**
			 * Will properly invalidate the referenced owner's Transform based on CoordinateSpace.
			 *
			 * @param t		Transform to reassign the one that this wrapper references
			 * @returns		this
			 */
			TransformWrapper& operator=(const Transform& t) noexcept requires (!IsConst)
			{
				owner.SetTransform<Space>(t);
				return *this;
			}
			
			[[nodiscard]] Vector3Wrapper<Space, Transform::Component::Translation, IsConst> Translation() noexcept;
			[[nodiscard]] Vector3Wrapper<Space, Transform::Component::Scale, IsConst> Scale() noexcept;
			[[nodiscard]] QuaternionWrapper<Space, IsConst> Rotation() noexcept;
		};
#pragma endregion
		
#pragma region iterator
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
			 * Does nothing if already at end().
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
#pragma endregion
		};

		class const_iterator final
		{
			friend class Entity;

			CONST_FORWARD_ITERATOR(const_iterator, iterator)
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
		[[nodiscard]] SharedPtr<const Entity> Parent() const noexcept;

		/**
		 * O(1)
		 *
		 * @param childName		the name of the child to query for
		 * @returns				this child with that name
		 */
		[[nodiscard]] SharedEntity Child(const String& childName) noexcept;

		/**
		 * O(1)
		 *
		 * @param childName		the name of the child to query for
		 * @returns				this child with that name
		 */
		[[nodiscard]] SharedPtr<const Entity> Child(const String& childName) const noexcept;
#pragma endregion

#pragma region Transform
		/**
		 * O(1) most cases
		 * Potentially O(n) if this is World space where n is the number of parents with invalid Transforms.
		 * 
		 * @param <Space>	CoordinateSpace this operation should take effect in.
		 * @returns			this Entity's local or world Transform
		 */
		template<CoordinateSpace Space>
		[[nodiscard]] const Transform& GetTransform() const noexcept;
		
		/**
		 * O(n) where n is the number of children.
		 * Sets a bool on all children to mark their Transforms invalid (comes into play in GetWorldTransform()).
		 *
		 * @param <Space>	CoordinateSpace this operation should take effect in.
		 * @param t			Transform to set this one's local or world to. 
		 */
		template<CoordinateSpace Space>
		void SetTransform(const Transform& t) noexcept;

		template<CoordinateSpace Space>
		TransformWrapper<Space, false> Transform() noexcept;

		template<CoordinateSpace Space>
		TransformWrapper<Space, true> Transform() const noexcept;
#pragma endregion
		
		/**
		 * O(1)
		 * 
		 * @returns		this Entity's name
		 */
		[[nodiscard]] constexpr const String& GetName() const noexcept;

		/**
		 * Will require re-hashing this Entity into its parent
		 * O(1)
		 * 
		 * @param newName	new name for this Entity
		 */
		void SetName(const String& newName) noexcept;
		
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
		SharedPtr<Derived> CreateChild(const String& childName, Args&&... args);

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
		SharedPtr<Derived> CreateChild(String&& childName, Args&&... args);
		
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
		SharedPtr<Derived> CreateChild(Args&&... args);

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
		SharedEntity Adopt(const String& childName, SharedEntity child);

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
		 * Doesn't take effect until the next Update().
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
		void RemoveChild(const String& childName) noexcept;
#pragma endregion

#pragma region operators
		/**
		 * O(n) where n is the number of attributes
		 *
		 * @param a		rhs
		 * @param b		lhs
		 * @returns		whether these Entities are the same
		 */
		friend bool operator==(const Entity& a, const Entity& b) noexcept;

		/**
		 * O(n) where n is the number of attributes
		 *
		 * @param a		rhs
		 * @param b		lhs
		 * @returns		whether these Entities are not the same
		 */
		friend bool operator!=(const Entity& a, const Entity& b) noexcept;
#pragma endregion

	protected:
		/**
		 * Invokes Init() on all children.
		 */
		virtual void Init()
		{
			for (auto& e : *this)
			{
				e->Init();
			}
		}

		/**
		 * Invokes Update() on all enabled children. 
		 */
		virtual void Update()
		{
			for (auto& e : *this)
			{			
				if (e->Enabled())
				{
					e->Update();
				}
			}
		}

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
