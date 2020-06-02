// MIT License Copyright (c) 2020 Jarrett Wendt

#pragma once
#include "Scope.h"

namespace Library
{
	class Attributed : public Scope
	{
		RTTI_DECLARATIONS(Scope)
		friend class Registry;

#pragma region Special Members
	public:
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

#pragma region Is

#pragma endregion

#pragma region ForEach

#pragma endregion

#pragma region Insert
		
#pragma endregion

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
	};
}

#define ATTRIBUTED_DECLARATIONS(BaseType)																		\
	RTTI_DECLARATIONS(BaseType)																					\
	friend class Library::Registry;

#define ATTRIBUTED_SPECIAL_MEMBERS(DerivedType)																	\
protected:																										\
	explicit DerivedType(RTTI::IDType derived) : Base(derived) {}												\
public:																											\
	DerivedType() : Base(typeID) {}																				\
	DerivedType(const DerivedType& other) : Base(other) {}														\
	DerivedType(DerivedType&& other) noexcept : Base(std::move(other)) {}										\
	DerivedType& operator=(const DerivedType& other) { Base::operator=(other); return *this; }					\
	DerivedType& operator=(DerivedType&& other) noexcept { Base::operator=(std::move(other)); return *this; }	\
	virtual ~DerivedType() = default;
	
		

#include "Attributed.inl"
