// MIT License Copyright (c) 2020 Jarrett Wendt

#pragma once

#include "Datum.h"
#include "RTTI.h"
#include "Macros.h"
#include "HashMap.h"

namespace Library
{
	class Registry;
	
	class Attributed : public RTTI
	{
		RTTI_DECLARATIONS(RTTI)
		friend class Registry;

	private:
		using MapType = HashMap<String, Datum>;

		/** where all the attributes are stored */
		MapType attributes{};

	public:
		class InvalidNameException final : public std::invalid_argument
		{
		public:
			explicit InvalidNameException(const String& str) : std::invalid_argument(str) {};
			SPECIAL_MEMBERS(InvalidNameException, default)
		};

#pragma region Special Members
		/**
		 * No default constructor. Must pass along the IDType of the derived. 
		 */
		Attributed() = delete;
	protected:
		/**
		 * Must be called by any derived constructor.
		 * 
		 * @param derived	the ID of the derived type
		 */
		explicit Attributed(RTTI::IDType derived);
	public:
		Attributed(const Attributed& other);
		Attributed(Attributed&& other) noexcept;
		Attributed& operator=(const Attributed& other);
		Attributed& operator=(Attributed&& other) noexcept;
		virtual ~Attributed() = 0;
#pragma endregion

#pragma region iterator
		using iterator = typename MapType::iterator;
		using const_iterator = typename MapType::const_iterator;

		BEGIN_END(iterator, const_iterator, Attributed)
#pragma endregion

#pragma region Properties
		/**
		 * @returns		how many attributes this Attributed has
		 */
		[[nodiscard]] constexpr size_t NumAttributes() const noexcept;

		/**
		 * @returns		if this Attributed has any attributes
		 */
		[[nodiscard]] constexpr bool HasAttributes() const noexcept;
#pragma endregion

#pragma region Accessors
		/**
		 * O(1)
		 *
		 * @param name	key to query for
		 * @returns		attribute at that key
		 *
		 * @throws std::out_of_range if no attribute with that key exists
		 */
		[[nodiscard]] Datum& Attribute(const String& name);

		/**
		 * O(1)
		 *
		 * @param name	key to query for
		 * @returns		attribute at that key
		 *
		 * @throws std::out_of_range if no attribute with that key exists
		 */
		[[nodiscard]] const Datum& Attribute(const String& name) const;
#pragma endregion

#pragma region Query

		/**
		 * O(1)
		 *
		 * @param name	key to query for
		 * @returns		whether or not an attribute of that key exists in this Attributed
		 */
		bool HasAttribute(const String& name) const noexcept;

		/**
		 * O(1)
		 * Searches within this Scope, not children or parents.
		 *
		 * @param name		name of child to query for
		 * @returns			iterator at that Scope, or end() if no such child exists
		 */
		[[nodiscard]] iterator Find(const String& name) noexcept;

		/**
		 * O(1)
		 * Searches within this Scope, not children or parents.
		 *
		 * @param name		name of child to query for
		 * @returns			iterator at that Scope, or end() if no such child exists
		 */
		[[nodiscard]] const_iterator Find(const String& name) const noexcept;
#pragma endregion

#pragma region Insert
		/**
		 * Does not overwrite any attribute already associated with name.
		 * O(1)
		 *
		 * @param name		name of the attribute
		 * @param datum		attribute to append, a copy of which will be made
		 * @returns			reference to newly inserted attribute
		 *
		 * @throws InvalidNameException
		 */
		Datum& AddAttribute(const String& name, const Datum& datum = {});

		/**
		 * Does not overwrite any attribute already associated with name.
		 * O(1)
		 *
		 * @param name		name of the attribute
		 * @param datum		attribute to append, a copy of which will be made
		 * @returns			reference to newly inserted attribute
		 *
		 * @throws InvalidNameException
		 */
		Datum& AddAttribute(const String& name, Datum&& datum);
#pragma endregion

#pragma region Remove
		/**
		 * Does nothing if no attribute by the specified name exists in this Attributed.
		 * Undefined behavior if the attribute being removed is prescribed.
		 * O(1)
		 *
		 * @param name		removes the attribute with the associated name
		 * @returns			whether or not there was an attribute by that name
		 */
		bool RemoveAttribute(const String & name) noexcept;
#pragma endregion

#pragma region operators
		/**
		 * O(n) where n is the number of attributes
		 *
		 * @param a		rhs
		 * @param b		lhs
		 * @returns		whether these Attributeds are the same
		 */
		friend bool operator==(const Attributed& a, const Attributed& b) noexcept;

		/**
		 * O(n) where n is the number of attributes
		 *
		 * @param a		rhs
		 * @param b		lhs
		 * @returns		whether these Attributeds are not the same
		 */
		friend bool operator!=(const Attributed& a, const Attributed& b) noexcept;

		/**
		 * Will Append a default constructed attribute if none exists with the specified key.
		 * O(1)
		 *
		 * @param name	name for attribute
		 * @returns		attribute at that name
		 *
		 * @throws InvalidNameException
		 */
		[[nodiscard]] Datum& operator[](const String& name);

		/**
		 * O(1)
		 *
		 * @param name	name for attribute
		 * @returns		attribute at that name
		 *
		 * @throws InvalidNameException
		 */
		[[nodiscard]] const Datum& operator[](const String& name) const;
#pragma endregion
		
#pragma region Helpers
	protected:
		/**
		 * @param name	the name to check
		 * @throws InvalidNameException
		 */
		static void ThrowName(const String& name);
		
	private:
		/**
		 * Recursively populates this Attributed's Scope with its and its base type's prescribed members.
		 * 
		 * @param derived	ID of the derived instance
		 */
		void Populate(RTTI::IDType derived);

		/**
		 * Performs the pointer fixup for all of the prescribed attributes.
		 * 
		 * @param derived	ID of the derived instance 
		 */
		void PointerFixup(IDType derived) noexcept;

		/**
		 * @returns		T* offset from this by byteOffset
		 */
		template<typename T = void>
		T* ByteOffsetThis(size_t byteOffset) const noexcept;
#pragma endregion
	};
}

#define ATTRIBUTED_DECLARATIONS(BaseType)								\
	RTTI_DECLARATIONS(BaseType)											\
	friend class Library::Registry;

#define ATTRIBUTED_CTORS(DerivedType)									\
protected:																\
	explicit DerivedType(const RTTI::IDType derived) : Base(derived) {}	\
public:																	\
	DerivedType() : Base(typeID) {}

#define ATTRIBUTED_SPECIAL_MEMBERS(DerivedType, D)						\
	ATTRIBUTED_CTORS(DerivedType)										\
	MOVE_COPY_VDTOR(DerivedType, D)

#include "Attributed.inl"
